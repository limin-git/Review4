#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"


ReviewManager::ReviewManager()
{
}


ReviewManager::~ReviewManager()
{
}


void ReviewManager::run()
{
    IInput::instance().add_key_handler( this, true, 'A', 'Z', boost::bind( &ReviewManager::continue_handler, this ) );
    IInput::instance().add_key_handler( this, true, VK_DELETE, boost::bind( &ReviewManager::disable_handler, this ) );
    m_slideshow = IScheduler::instance().get_slideshow();
    m_slideshow->show();
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
    m_slideshow = IScheduler::instance().get_slideshow();

    if ( m_slideshow->show() )
    {
        m_slideshow = IScheduler::instance().get_slideshow();
    }
}


void ReviewManager::back_handler()
{
}


void ReviewManager::disable_handler()
{
    IDisable::instance().disable( m_slideshow );
    m_slideshow = IScheduler::instance().get_slideshow();
    m_slideshow->show();
}
