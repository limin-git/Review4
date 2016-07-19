#include "stdafx.h"
#include "TextReview.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"
#include "IConfigurationFile.h"


TextReview::TextReview()
    : m_current_show_finished( false )
{
}


TextReview::~TextReview()
{
    handle_exit();
}


void TextReview::handle_start()
{
    m_review_history.push_back( IScheduler::instance().get_slideshow() );
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
        m_review_history.push_back( IScheduler::instance().get_slideshow() );
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


void TextReview::handle_jump( size_t distance )
{
    handle_next();
}


void TextReview::handle_jump_back( size_t distance )
{
    handle_previous();
}
