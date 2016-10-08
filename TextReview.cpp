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

#define DEFAULT_REVIEW_AGAIN_DISTANCE   30
#define DEFAULT_LISTEN_INTERVAL         1500
#define review_review_again_distance    "review.review-again-distance"
#define review_auto_review_again        "review.auto-review-again"
#define review_listen_interval          "review.listen-interval"



TextReview::TextReview()
    : m_current_show_finished( false ),
      m_index( 0 ),
      m_review_again_distance( DEFAULT_REVIEW_AGAIN_DISTANCE ),
      m_listen_interval( DEFAULT_LISTEN_INTERVAL ),
      m_listening( false ),
      m_auto_review_again( false )
{
    po::options_description options;
    options.add_options()
        ( review_review_again_distance, po::value<size_t>(), "review again distance" )
        ( review_auto_review_again,     po::wvalue<std::wstring>(), "auto add to review again list" )
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
    m_current = m_review_history.begin();
    show();
}


void TextReview::handle_exit()
{
}


void TextReview::handle_continue()
{
    show();
}


void TextReview::handle_next()
{
    go_forward();
    m_current_show_finished = false;
    show();
}


void TextReview::handle_previous()
{
    go_back();
    m_current_show_finished = false;
    show();
}


void TextReview::handle_replay()
{
    (*m_current)->clear_state();
    m_current_show_finished = (*m_current)->show();
}


void TextReview::handle_disable()
{
    ISlideshowPtr slideshow = *m_current;
    handle_next();
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

    m_current_show_finished = (*m_current)->show();
}


void TextReview::go_forward()
{
    if ( (*m_current)->empty() )
    {
        (*m_current) = IScheduler::instance().get_slideshow();
    }
    else
    {
        m_current++;
    }

    if ( ( false == m_review_again.empty() ) && ( m_review_again_distance <= m_index - m_review_again.front().first ) )
    {
        m_review_history.push_back( m_review_again.front().second );
        m_review_again_set.erase( m_review_again.front().second->key() );
        m_review_again.pop_front();
        m_current = m_review_history.end();
        m_current--;
    }
    else if ( m_current == m_review_history.end() )
    {
        m_review_history.push_back( IScheduler::instance().get_slideshow() );
        m_index++;
        m_current = m_review_history.end();
        m_current--;
    }

    (*m_current)->clear_state();

    if ( m_auto_review_again )
    {
        size_t key = (*m_current)->key();

        if ( m_review_again_set.find( key ) == m_review_again_set.end() )
        {
            m_review_again.push_back( std::make_pair( m_index, *m_current ) );
            m_review_again_set.insert( key );
        }
    }
}


void TextReview::go_back()
{
    if ( m_current != m_review_history.begin() )
    {
        m_current--;
    }

    (*m_current)->clear_state();
}


void TextReview::delete_review_history( size_t key )
{
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
    if ( ( m_current != m_review_history.end() ) && ! (*m_current)->empty() )
    {
        if ( m_review_again.empty() || m_review_again.back().first != m_index )
        {
            (*m_current)->clear_state();
            m_review_again.push_back( std::make_pair( m_index, *m_current ) );
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
    if ( ( m_current != m_review_history.end() ) && ! (*m_current)->empty() )
    {
        IFilter::instance().filter( *m_current );
        handle_disable();
    }
}


void TextReview::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<size_t>( review_review_again_distance, vm, old ) )
    {
        m_review_again_distance = vm[review_review_again_distance].as<size_t>();
    }

    if ( Utility::updated<std::wstring>( review_auto_review_again, vm, old ) )
    {
        m_auto_review_again = ( L"true" == vm[review_auto_review_again].as<std::wstring>() );
    }

    if ( Utility::updated<size_t>( review_listen_interval, vm, old ) )
    {
        m_listen_interval = vm[review_listen_interval].as<size_t>();
    }
}


void TextReview::listen_thread_function()
{
    bool is_auto_review = m_auto_review_again;
    bool is_syn = IEnglishPlayer::instance().is_synchronized();

    IEnglishPlayer::instance().synchronize( true );
    m_auto_review_again = false;

    while ( m_listening )
    {
        (*m_current)->show_all();       if ( !m_listening ) { break; }

        if ( m_listen_interval )
        {
            Sleep( m_listen_interval );     if ( !m_listening ) { break; }
        }

        if ( (*m_current)->empty() )
        {
            exit(0);
        }

        go_forward();                   if ( !m_listening ) { break; }
    }

    m_auto_review_again = is_auto_review;
    IEnglishPlayer::instance().synchronize( is_syn );
}
