#include "stdafx.h"
#include "TextReview.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"
#include "IConfigurationFile.h"
#include "Utility.h"
#include "IFilter.h"
#include "IEnglishPlayer.h"
#include "IGlobalSignals.h"
#include "EmptySlideshow.h"
#include "ILog.h"
#include "TlsParameter.h"

#define DEFAULT_REVIEW_AGAIN_DISTANCE   30
#define DEFAULT_LISTEN_INTERVAL         1500
#define review_review_again_distance    "review.review-again-distance"
#define review_auto_review_again        "review.auto-review-again"
#define review_listen_interval          "review.listen-interval"


struct AsynchronizedGuard
{
    AsynchronizedGuard( bool is_listening )
        : m_is_listening( is_listening )
    {
        if ( is_listening )
        {
            m_is_synchronized = IEnglishPlayer::instance().is_synchronized();
            IEnglishPlayer::instance().synchronize( false );
        }
    }

    ~AsynchronizedGuard()
    {
        if ( m_is_listening )
        {
            IEnglishPlayer::instance().synchronize( m_is_synchronized );
        }
    }

    bool m_is_listening;
    bool m_is_synchronized;
};



TextReview::TextReview()
    : m_current_show_finished( false ),
      m_index( 0 ),
      m_review_again_distance( DEFAULT_REVIEW_AGAIN_DISTANCE ),
      m_listen_interval( DEFAULT_LISTEN_INTERVAL ),
      m_listening( false ),
      m_auto_review_again( 0 )
{
    po::options_description options;
    options.add_options()
        ( review_review_again_distance, po::value<size_t>(), "review again distance" )
        ( review_auto_review_again,     po::value<size_t>(), "auto add to review again list" )
        ( review_listen_interval,       po::value<size_t>(), "review listen interval" )
        ;
    IConfigurationFile::instance().add_options_description( options ).add_observer( this );
}


TextReview::~TextReview()
{
    handle_exit();
}


void TextReview::handle_start()
{
    m_review_history.push_back( IScheduler::instance().get_slideshow() );
    m_index++;
    m_current_it = m_review_history.begin();
    m_current_slide = *m_current_it;
    show();
}


void TextReview::handle_exit()
{
}


void TextReview::handle_continue()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );
    disable_play_when_listening();
    show();
}


void TextReview::handle_next()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );
    disable_play_when_listening();
    go_forward();
    m_current_show_finished = false;
    show();
}


void TextReview::handle_previous()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );
    disable_play_when_listening();
    go_back();
    m_current_show_finished = false;
    show();
}


void TextReview::handle_replay()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );
    m_current_slide->clear_state();
    m_current_show_finished = m_current_slide->show();
}


void TextReview::handle_disable()
{
    ISlideshowPtr slideshow = m_current_slide;

    if ( ! m_listening )
    {
        handle_next();
    }

    size_t key = slideshow->key();
    delete_review_history( key );
    IDisable::instance().disable( slideshow );
}


void TextReview::handle_jump( size_t distance )
{
    handle_next();
}


void TextReview::handle_jump_back( size_t distance )
{
    handle_previous();
}


void TextReview::show()
{
    if ( m_current_show_finished )
    {
        go_forward();
    }

    AsynchronizedGuard guard( m_listening );
    m_current_show_finished = m_current_slide->show();
}


void TextReview::go_forward()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );

    // inside history
    if ( ++m_current_it != m_review_history.end() )
    {
        m_current_slide = *m_current_it;
        m_current_slide->clear_state();
        return;
    }

    if ( ! m_review_again.empty() && ( m_review_again_distance <= m_index - m_review_again.front().first ) )
    {
        ISlideshowPtr slide = m_review_again.front().second;
        m_review_history.push_back( slide );
        m_review_again.pop_front();
        m_review_again_set.erase( slide->key() );
        m_current_it = m_review_history.end();
        m_current_it--;
    }
    else
    {
        ISlideshowPtr slide = IScheduler::instance().get_slideshow();

        if ( slide->empty() )
        {
            if ( ! m_review_again.empty() )
            {
                m_current_slide = m_review_again.front().second;
                m_review_again.pop_front();
                m_review_again.push_back( std::make_pair( m_index, m_current_slide ) );
            }
            else
            {
                m_current_slide = slide;
            }

            if ( m_current_it != m_review_history.begin() )
            {
                m_current_it--;
            }

            return;
        }

        m_review_history.push_back( slide );
        m_index++;
        m_current_it = m_review_history.end();
        m_current_it--;
    }

    m_current_slide = *m_current_it;
    m_current_slide->clear_state();

    if ( m_auto_review_again )
    {
        size_t key = m_current_slide->key();

        if ( m_review_times[key] < m_auto_review_again )
        {
            if ( m_review_again_set.find( key ) == m_review_again_set.end() )
            {
                m_review_again.push_back( std::make_pair( m_index, m_current_slide ) );
                m_review_again_set.insert( key );
                m_review_times[key]++;
            }
        }
    }
}


void TextReview::go_back()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );

    if ( m_current_it != m_review_history.begin() )
    {
        m_current_it--;
        m_current_slide = *m_current_it;
    }

    m_current_slide->clear_state();
}


void TextReview::delete_review_history( size_t key )
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );

    while ( ( m_current_it != m_review_history.end() ) && ( m_current_slide->key() == key ) )
    {
        m_current_it++;

        if ( m_current_it != m_review_history.end() )
        {
            m_current_slide = *m_current_it;
        }
        else
        {
            m_current_it--;
            go_forward();
        }
    }

    for ( ISlideshowPtrList::iterator it = m_review_history.begin(); it != m_review_history.end(); NULL )
    {
        if ( (*it)->key() == key )
        {
            m_review_history.erase( it++ );
            continue;
        }

        it++;
    }

    for ( std::list< std::pair<int, ISlideshowPtr> >::iterator it = m_review_again.begin(); it != m_review_again.end(); NULL )
    {
        if ( it->second->key() == key )
        {
            m_review_again.erase( it++ );
            continue;
        }

        it++;
    }

    m_review_again_set.erase( key );
}


void TextReview::handle_review_again()
{
    boost::lock_guard<boost::recursive_mutex> guard( m_mutex );

    if ( ( m_current_it != m_review_history.end() ) && ! m_current_slide->empty() )
    {
        if ( m_review_again.empty() || m_review_again.back().first != m_index )
        {
            m_current_slide->clear_state();
            m_review_again.push_back( std::make_pair( m_index, m_current_slide ) );
        }
    }

    if ( !m_listening )
    {
        handle_next();
    }
}


void TextReview::handle_listen()
{
    m_listening = !m_listening;

    if ( m_listening )
    {
        boost::thread t( boost::bind( &TextReview::listen_thread_function, this ) );
    }
}


void TextReview::handle_filter()
{
    if ( ( m_current_it != m_review_history.end() ) && ! (*m_current_it)->empty() )
    {
        IFilter::instance().filter( *m_current_it );
        handle_disable();
    }
}


void TextReview::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<size_t>( review_review_again_distance, vm, old ) )
    {
        m_review_again_distance = vm[review_review_again_distance].as<size_t>();
    }

    if ( Utility::updated<size_t>( review_auto_review_again, vm, old ) )
    {
        m_auto_review_again = vm[review_auto_review_again].as<size_t>();
    }

    if ( Utility::updated<size_t>( review_listen_interval, vm, old ) )
    {
        m_listen_interval = vm[review_listen_interval].as<size_t>();
    }
}


void TextReview::listen_thread_function()
{
    bool is_syn = IEnglishPlayer::instance().is_synchronized();
    IEnglishPlayer::instance().synchronize( true );

    while ( m_listening )
    {
        m_current_slide->show_all(); if ( !m_listening ) { break; }
        IGlobalSignals::instance().signal_next_slide();

        if ( m_listen_interval )
        {
            for ( size_t i = 0, step = 10; m_listening && i < m_listen_interval; i += step )
            {
                Sleep( step );
            }

            if ( !m_listening ) { break; }
        }

        if ( m_current_slide->empty() )
        {
            exit(0);
        }

        go_forward(); if ( !m_listening ) { break; }

        if ( m_current_slide->empty() )
        {
            exit(0);
        }
    }

    IGlobalSignals::instance().signal_next_slide();
    IEnglishPlayer::instance().synchronize( is_syn );
}


void TextReview::disable_play_when_listening()
{
    boost::thread_specific_ptr<std::wstring>& param = TlsParameter::get_param( TLS_PARAM_PlayEnglish );

    if ( m_listening && NULL == param.get() )
    {
        param.reset( new std::wstring( L"false" ) );
    }
    else if ( ! m_listening && param.get() != NULL )
    {
        param.reset();
    }
}
