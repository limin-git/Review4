#include "stdafx.h"
#include "TextReview.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"
#include "IConfigurationFile.h"

#define DEFAULT_REVIEW_AGAIN_DISTANCE   30
#define review_review_again_distance    "review.review-again-distance"


TextReview::TextReview()
    : m_current_show_finished( false ),
      m_index( 0 ),
      m_review_again_distance( DEFAULT_REVIEW_AGAIN_DISTANCE )
{
    po::options_description options;
    options.add_options()
        ( review_review_again_distance, po::value<size_t>(), "review again distance" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).variables_map();

    if ( vm.count( review_review_again_distance ) )
    {
        m_review_again_distance = vm[review_review_again_distance].as<size_t>();
    }
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
    size_t key = slideshow->key();
    handle_next();
    IDisable::instance().disable( slideshow );
    delete_review_history( key );
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

    if ( m_current == m_review_history.end() )
    {
        if ( ( false == m_review_again.empty() ) && ( m_review_again_distance <= m_index - m_review_again.front().first ) )
        {
            m_review_history.push_back( m_review_again.front().second );
            m_review_again.pop_front();
        }
        else
        {
            m_review_history.push_back( IScheduler::instance().get_slideshow() );
            m_index++;
        }

        m_current = m_review_history.end();
        m_current--;
    }
    else
    {
        (*m_current)->clear_state();
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

    handle_next();
}


void TextReview::handle_listen()
{
    struct ListenThread
    {
        ListenThread( TextReview* review )
            : listening( false ),
              m_review( review )
        {
        }

        void listen()
        {
            listening = true;

            m_review->handle_next();

            while ( listening )
            {
                m_review->handle_continue();

                if ( listening )
                {
                    Sleep( 1500 );
                }

                if ( listening )
                {
                    m_review->handle_continue();
                }
            }
        }

        bool listening;
        TextReview* m_review;
    };

    static ListenThread* listen_thread = new ListenThread( this );

    if ( false == listen_thread->listening )
    {
        boost::thread t( boost::bind( &ListenThread::listen, listen_thread ) );
    }
    else
    {
        listen_thread->listening = false;
    }
}
