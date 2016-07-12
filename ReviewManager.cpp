#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IScheduler.h"
#include "IDisable.h"
#include "IHotKey.h"
#include "IConfigurationFile.h"
#include "ILog.h"
#include "TextReview.h"
#include "SubtitleReview.h"
#include "Wallpaper.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

// singleton dependency:
// IReviewManager
//     --> IInput
//     --> IScheduler
//     --> IDisable
//     --> IConsole


ReviewManager::ReviewManager()
    : m_register_hot_keys( false )
{
    po::options_description options( "" );
    options.add_options()
        ( "advanced.register-hot-keys", po::wvalue<std::wstring>(), "register hot keys?" )
        ( "file.name", po::wvalue<std::wstring>(), "file name" )
        ( "wallpaper.path", po::wvalue<std::wstring>(), "register hot keys?" )
        ;
    po::variables_map& vm = IConfigurationFile::instance()->add_options_description( options ).variables_map();

    if ( vm.count( "advanced.register-hot-keys" ) )
    {
        m_register_hot_keys = L"true" == vm["advanced.register-hot-keys"].as<std::wstring>();
    }

    if ( vm.count( "file.name" ) )
    {
        fs::path file_name = fs::system_complete( vm["file.name"].as<std::wstring>() );

        if ( file_name.extension() == ".ece" )
        {
            m_reivews.insert( new TextReview );
        }
        else if ( file_name.extension() == ".srt" )
        {
            m_reivews.insert( new SubtitleReview );
        }
        else if ( file_name.extension() == ".ass" )
        {
            m_reivews.insert( new SubtitleReview );
        }
    }

    if ( vm.count( "wallpaper.path" ) )
    {
        m_reivews.insert( new Wallpaper );
    }
}


ReviewManager::~ReviewManager()
{
    m_input->remove_key_handler( this ).remove_mouse_handler( this );
    m_hotkey->unregister_handler( this );

    BOOST_FOREACH( IReview* review, m_reivews )
    {
        delete review;
    }
}


void ReviewManager::run()
{
    start();

    if ( m_register_hot_keys )
    {
        m_hotkey
           ->register_handler( this, 0, VK_DOWN,                boost::bind( &ReviewManager::handle_continue, this ) )
            .register_handler( this, 0, VK_LEFT,                boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, VK_PRIOR,               boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, VK_RIGHT,               boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, VK_NEXT,                boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, VK_UP,                  boost::bind( &ReviewManager::handle_replay, this ) )
            .register_handler( this, 0, VK_DELETE,              boost::bind( &ReviewManager::handle_disable, this ) )
            .register_handler( this, MOD_CONTROL, VK_RIGHT,     boost::bind( &ReviewManager::handle_jump, this, 10 ) )
            .register_handler( this, MOD_CONTROL, VK_DOWN,      boost::bind( &ReviewManager::handle_jump, this, 50 ) )
            .register_handler( this, MOD_CONTROL, VK_NEXT,      boost::bind( &ReviewManager::handle_jump, this, 100 ) )
            .register_handler( this, MOD_CONTROL, VK_LEFT,      boost::bind( &ReviewManager::handle_jump_back, this, 10 ) )
            .register_handler( this, MOD_CONTROL, VK_UP,        boost::bind( &ReviewManager::handle_jump_back, this, 50 ) )
            .register_handler( this, MOD_CONTROL, VK_PRIOR,     boost::bind( &ReviewManager::handle_jump_back, this, 100 ) )
            ;
    }
    else
    {
        m_input
           ->add_key_handler( this, 0, VK_DOWN,      boost::bind( &ReviewManager::handle_continue, this ) )
            .add_key_handler( this, 0, VK_BACK,      boost::bind( &ReviewManager::handle_continue, this ) )
            .add_key_handler( this, 0, VK_OEM_3,     boost::bind( &ReviewManager::handle_continue, this ) )     // '`~' for US
            .add_key_handler( this, 0, VK_OEM_5,     boost::bind( &ReviewManager::handle_continue, this ) )     //  '\|' for US
            .add_key_handler( this, 0, VK_RIGHT,     boost::bind( &ReviewManager::handle_next, this ) )
            .add_key_handler( this, 0, VK_NEXT,      boost::bind( &ReviewManager::handle_next, this ) )
            .add_key_handler( this, 0, VK_UP,        boost::bind( &ReviewManager::handle_replay, this ) )
            .add_key_handler( this, 0, VK_LEFT,      boost::bind( &ReviewManager::handle_previous, this ) )
            .add_key_handler( this, 0, VK_PRIOR,     boost::bind( &ReviewManager::handle_previous, this ) )
            .add_key_handler( this, 0, VK_DELETE,    boost::bind( &ReviewManager::handle_disable, this ) )
            ;
    }

    m_input->run();
}


void ReviewManager::start()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_start, review ) );
    }
}


void ReviewManager::handle_continue()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_continue, review ) );
    }
}


void ReviewManager::handle_next()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_next, review ) );
    }
}


void ReviewManager::handle_previous()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_previous, review ) );
    }
}


void ReviewManager::handle_replay()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_replay, review ) );
    }
}


void ReviewManager::handle_disable()
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_disable, review ) );
    }
}


void ReviewManager::handle_jump( size_t distance )
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_jump, review, distance ) );
    }
}


void ReviewManager::handle_jump_back( size_t distance )
{
    BOOST_FOREACH( IReview* review, m_reivews )
    {
        m_thread_pool.queue_item( boost::bind( &IReview::handle_jump_back, review, distance ) );
    }
}
