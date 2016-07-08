#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"

// singleton dependency:
// IReviewManager
//     --> IInput
//     --> IScheduler
//     --> IDisable
//     --> IConsole


ReviewManager::ReviewManager()
    : m_forward_before_show( false )
{
}


ReviewManager::~ReviewManager()
{
    IInput::instance()
        .remove_key_handler( this )
        .remove_mouse_handler( this )
        ;
    IHotKey::instance().unregister_handler( this );
}


void ReviewManager::run()
{
    m_slideshow_history.push_back( IScheduler::instance().get_slideshow() );
    m_current = m_slideshow_history.begin();

    show();

    IInput::instance()
        .add_key_handler( this, true, VK_DOWN,      boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_BACK,      boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_RETURN,    boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_OEM_3,     boost::bind( &ReviewManager::continue_handler, this ) )     // '`~' for US
        .add_key_handler( this, true, VK_OEM_5,     boost::bind( &ReviewManager::continue_handler, this ) )     //  '\|' for US
        .add_key_handler( this, true, VK_RIGHT,     boost::bind( &ReviewManager::next_handler, this ) )
        .add_key_handler( this, true, VK_NEXT,      boost::bind( &ReviewManager::next_handler, this ) )
        .add_key_handler( this, true, VK_UP,        boost::bind( &ReviewManager::repeat_handler, this ) )
        .add_key_handler( this, true, VK_LEFT,      boost::bind( &ReviewManager::previous_handler, this ) )
        .add_key_handler( this, true, VK_PRIOR,     boost::bind( &ReviewManager::previous_handler, this ) )
        .add_key_handler( this, true, VK_DELETE,    boost::bind( &ReviewManager::disable_handler, this ) )
        .add_mouse_handler( this, 0, FROM_LEFT_1ST_BUTTON_PRESSED,  boost::bind( &ReviewManager::continue_handler, this ) )
        .add_mouse_handler( this, MOUSE_HWHEELED, 0,                boost::bind( &ReviewManager::continue_handler, this ) )
        .add_mouse_handler( this, 0, RIGHTMOST_BUTTON_PRESSED,      boost::bind( &ReviewManager::previous_handler, this ) )
        ;
    IHotKey::instance()
        .register_handler( this, 0, VK_LEFT,    boost::bind( &ReviewManager::previous_handler, this ) )
        .register_handler( this, 0, VK_RIGHT,   boost::bind( &ReviewManager::next_handler, this ) )
        .register_handler( this, 0, VK_UP,      boost::bind( &ReviewManager::repeat_handler, this ) )
        .register_handler( this, 0, VK_DOWN,    boost::bind( &ReviewManager::continue_handler, this ) )
        ;
}


void ReviewManager::continue_handler()
{
    show();
}


void ReviewManager::next_handler()
{
    forward();
    m_forward_before_show = false;
    show();
}


void ReviewManager::previous_handler()
{
    if ( m_current != m_slideshow_history.begin() )
    {
        m_current--;
        (*m_current)->clear_state();
        m_forward_before_show = false;
        show();
    }
}


void ReviewManager::show()
{
    if ( m_forward_before_show )
    {
        forward();
    }

    m_forward_before_show = (*m_current)->show();
}


void ReviewManager::forward()
{
    if ( (*m_current)->empty() )
    {
        (*m_current) = IScheduler::instance().get_slideshow();
    }
    else
    {
        m_current++;
    }

    if ( m_current == m_slideshow_history.end() )
    {
        m_slideshow_history.push_back( IScheduler::instance().get_slideshow() );
        m_current = m_slideshow_history.end();
        m_current--;
    }
    else
    {
        (*m_current)->clear_state();
    }
}


void ReviewManager::repeat_handler()
{
    (*m_current)->clear_state();
    m_forward_before_show = (*m_current)->show();
}


void ReviewManager::disable_handler()
{
    ISlideshowPtr slideshow = *m_current;
    size_t key = slideshow->key();
    next_handler();
    IDisable::instance().disable( slideshow );
    delete_review_history( key );
}


void ReviewManager::delete_review_history( size_t key )
{
    for ( ISlideshowPtrList::iterator it = m_slideshow_history.begin(); it != m_slideshow_history.end(); NULL )
    {
        if ( (*it)->key() == key )
        {
            m_slideshow_history.erase( it++ );
            continue;
        }

        it++;
    }
}
