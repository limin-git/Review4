#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"
#include "IConfigurationFile.h"
#include "ILog.h"

namespace po = boost::program_options;

// singleton dependency:
// IReviewManager
//     --> IInput
//     --> IScheduler
//     --> IDisable
//     --> IConsole


ReviewManager::ReviewManager()
    : m_current_show_finished( false ),
      m_register_hot_keys( false )
{
    po::options_description options( "" );
    options.add_options()
        ( "advanced.register-hot-keys", po::wvalue<std::wstring>(), "register hot keys?" )
        ;
    po::variables_map& vm = m_configuration->add_options_description( options ).variables_map();

    if ( vm.count( "advanced.register-hot-keys" ) )
    {
        m_register_hot_keys = L"true" == vm["advanced.register-hot-keys"].as<std::wstring>();
    }
}


ReviewManager::~ReviewManager()
{
    m_input->remove_key_handler( this ).remove_mouse_handler( this );
    m_hotkey->unregister_handler( this );
}


void ReviewManager::run()
{
    m_review_history.push_back( m_scheduler->get_slideshow() );
    m_current = m_review_history.begin();

    show();

    m_input
        ->add_key_handler( this, 0, VK_DOWN,     boost::bind( &ReviewManager::handle_continue, this ) )
        .add_key_handler( this, 0, VK_BACK,      boost::bind( &ReviewManager::handle_continue, this ) )
        .add_key_handler( this, 0, VK_OEM_3,     boost::bind( &ReviewManager::handle_continue, this ) )     // '`~' for US
        .add_key_handler( this, 0, VK_OEM_5,     boost::bind( &ReviewManager::handle_continue, this ) )     //  '\|' for US
        .add_key_handler( this, 0, VK_RIGHT,     boost::bind( &ReviewManager::handle_next, this ) )
        .add_key_handler( this, 0, VK_NEXT,      boost::bind( &ReviewManager::handle_next, this ) )
        .add_key_handler( this, 0, VK_UP,        boost::bind( &ReviewManager::handle_replay, this ) )
        .add_key_handler( this, 0, VK_LEFT,      boost::bind( &ReviewManager::handle_previous, this ) )
        .add_key_handler( this, 0, VK_PRIOR,     boost::bind( &ReviewManager::handle_previous, this ) )
        .add_key_handler( this, 0, VK_DELETE,    boost::bind( &ReviewManager::handle_disable, this ) )
        .add_mouse_handler( this, 0, FROM_LEFT_1ST_BUTTON_PRESSED,  boost::bind( &ReviewManager::handle_continue, this ) )
        .add_mouse_handler( this, MOUSE_HWHEELED, 0,                boost::bind( &ReviewManager::handle_continue, this ) )
        .add_mouse_handler( this, 0, RIGHTMOST_BUTTON_PRESSED,      boost::bind( &ReviewManager::handle_previous, this ) )
        ;

    if ( m_register_hot_keys )
    {
        m_hotkey
            ->register_handler( this, 0, VK_LEFT,               boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, VK_RIGHT,               boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, VK_UP,                  boost::bind( &ReviewManager::handle_replay, this ) )
            .register_handler( this, 0, VK_DOWN,                boost::bind( &ReviewManager::handle_continue, this ) )
            .register_handler( this, 0, VK_DELETE,              boost::bind( &ReviewManager::handle_disable, this ) )
            .register_handler( this, MOD_CONTROL, VK_RIGHT,     boost::bind( &ReviewManager::handle_jump, this, 10 ) )
            .register_handler( this, MOD_CONTROL, VK_DOWN,      boost::bind( &ReviewManager::handle_jump, this, 50 ) )
            .register_handler( this, MOD_CONTROL, VK_NEXT,      boost::bind( &ReviewManager::handle_jump, this, 100 ) )
            .register_handler( this, MOD_CONTROL, VK_LEFT,      boost::bind( &ReviewManager::handle_jump_back, this, 10 ) )
            .register_handler( this, MOD_CONTROL, VK_UP,        boost::bind( &ReviewManager::handle_jump_back, this, 50 ) )
            .register_handler( this, MOD_CONTROL, VK_PRIOR,     boost::bind( &ReviewManager::handle_jump_back, this, 100 ) )
            ;
    }
}


void ReviewManager::handle_continue()
{
    show();
}


void ReviewManager::handle_next()
{
    go_forward();
    m_current_show_finished = false;
    show();
}


void ReviewManager::handle_previous()
{
    go_back();
    m_current_show_finished = false;
    show();
}


void ReviewManager::show()
{
    if ( m_current_show_finished )
    {
        go_forward();
    }

    m_current_show_finished = (*m_current)->show();
}


void ReviewManager::go_forward()
{
    if ( (*m_current)->empty() )
    {
        (*m_current) = m_scheduler->get_slideshow();
    }
    else
    {
        m_current++;
    }

    if ( m_current == m_review_history.end() )
    {
        m_review_history.push_back( m_scheduler->get_slideshow() );
        m_current = m_review_history.end();
        m_current--;
    }
    else
    {
        (*m_current)->clear_state();
    }
}


void ReviewManager::go_back()
{
    if ( m_current != m_review_history.begin() )
    {
        m_current--;
        (*m_current)->clear_state();
    }
}


void ReviewManager::handle_replay()
{
    (*m_current)->clear_state();
    m_current_show_finished = (*m_current)->show();
}


void ReviewManager::handle_disable()
{
    ISlideshowPtr slideshow = *m_current;
    size_t key = slideshow->key();
    handle_next();
    m_disable->disable( slideshow );
    delete_review_history( key );
}


void ReviewManager::delete_review_history( size_t key )
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


void ReviewManager::handle_jump( size_t distance )
{
    Sleep( 500 );

    for ( size_t i = 0; i < distance + 1; ++i )
    {
        go_forward();
    }

    m_current_show_finished = false;
    show();
}


void ReviewManager::handle_jump_back( size_t distance )
{
    Sleep( 500 );

    for ( size_t i = 0; i < distance + 1; ++i )
    {
        go_back();
    }

    m_current_show_finished = false;
    show();
}
