#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"


ReviewManager::ReviewManager()
{
}


ReviewManager::~ReviewManager()
{
}


void ReviewManager::run()
{
    IInput::instance().add_key_handler( this, true, 'A', 'Z', boost::bind( &ReviewManager::continue_handler, this ) );
}


void ReviewManager::continue_handler()
{
    if ( !m_slideshow )
    {
        m_slideshow = IScheduler::instance().get_slideshow();
    }

    if ( m_slideshow->show() )
    {
        m_slideshow = IScheduler::instance().get_slideshow();
    }
}


void ReviewManager::next_handler()
{
}


void ReviewManager::back_handler()
{
}
