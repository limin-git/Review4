#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"

// singleton dependency:
// IReviewManager
//     --> IInput
//     --> IScheduler
//     --> IDisable
//     --> IConsole


ReviewManager::ReviewManager()
{
}


ReviewManager::~ReviewManager()
{
    IInput::instance()
        .remove_key_handler( this )
        .remove_mouse_handler( this );
}


void ReviewManager::run()
{
    IInput::instance()
        .add_key_handler( this, true, 'A', 'Z', boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_SPACE, boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_RIGHT, boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_DOWN,  boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_NEXT,  boost::bind( &ReviewManager::continue_handler, this ) )
        .add_key_handler( this, true, VK_DELETE, boost::bind( &ReviewManager::disable_handler, this ) )
        ;
    m_slideshow = IScheduler::instance().get_slideshow();
    show();
}


void ReviewManager::continue_handler()
{
    show();
}


void ReviewManager::next_handler()
{
    m_slideshow = IScheduler::instance().get_slideshow();
    show();
}


void ReviewManager::back_handler()
{
}


void ReviewManager::disable_handler()
{
    IDisable::instance().disable( m_slideshow );
    m_slideshow = IScheduler::instance().get_slideshow();
    show();
}


void ReviewManager::show()
{
    if ( m_slideshow->show() )
    {
        m_slideshow = IScheduler::instance().get_slideshow();
    }
}
