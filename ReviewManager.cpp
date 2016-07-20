#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IHotKey.h"
#include "IInputSender.h"
#include "IConfigurationFile.h"
#include "TextReview.h"
#include "SubtitleReview.h"
#include "Wallpaper.h"
namespace po = boost::program_options;
namespace fs = boost::filesystem;


ReviewManager::ReviewManager()
    : m_register_hot_keys( false )
{
    po::options_description options( "" );
    options.add_options()
        ( "advanced.register-hot-keys", po::wvalue<std::wstring>(), "register hot keys?" )
        ( "file.name", po::wvalue<std::wstring>(), "file name" )
        ( "wallpaper.path", po::wvalue<std::wstring>(), "register hot keys?" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).variables_map();

    if ( vm.count( "advanced.register-hot-keys" ) )
    {
        m_register_hot_keys = ( L"true" == vm["advanced.register-hot-keys"].as<std::wstring>() );
    }

    if ( vm.count( "file.name" ) )
    {
        fs::path file_name = fs::system_complete( vm["file.name"].as<std::wstring>() );

        if ( file_name.extension() == ".ece" )
        {
            m_reivews[new TextReview] = new QueueProcessor<>;
        }
        else if ( file_name.extension() == ".srt" )
        {
            m_reivews[new SubtitleReview] = new QueueProcessor<>;
        }
        else if ( file_name.extension() == ".ass" )
        {
            m_reivews[new SubtitleReview] = new QueueProcessor<>;
        }
    }

    if ( vm.count( "wallpaper.path" ) )
    {
        m_reivews[new Wallpaper] = new QueueProcessor<1>;
    }
}


ReviewManager::~ReviewManager()
{
    //IInput::instance().remove_key_handler( this ).remove_mouse_handler( this );
    //IHotKey::instance().unregister_handler( this );

    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        delete v.first;
        delete v.second;
    }
}


void ReviewManager::run()
{
    start();

    if ( m_register_hot_keys )
    {
        IHotKey::instance()
            .register_handler( this, 0, VK_DOWN,                boost::bind( &ReviewManager::handle_continue, this ) )
            .register_handler( this, 0, VK_OEM_3,               boost::bind( &ReviewManager::handle_continue, this ) )     // '`~' for US
            .register_handler( this, 0, VK_OEM_5,               boost::bind( &ReviewManager::handle_continue, this ) )     //  '\|' for US
            .register_handler( this, 0, VK_LEFT,                boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, VK_PRIOR,               boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, VK_RIGHT,               boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, VK_NEXT,                boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, VK_UP,                  boost::bind( &ReviewManager::handle_replay, this ) )
            .register_handler( this, 0, VK_DELETE,              boost::bind( &ReviewManager::handle_disable, this ) )
            .register_handler( this, 0, VK_ESCAPE,              boost::bind( &ReviewManager::handle_exit, this ) )
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
        IInput::instance()
            .add_key_handler( this, 0, VK_DOWN,      boost::bind( &ReviewManager::handle_continue, this ) )
            .add_key_handler( this, 0, VK_BACK,      boost::bind( &ReviewManager::handle_continue, this ) )
            .add_key_handler( this, 0, VK_OEM_3,     boost::bind( &ReviewManager::handle_continue, this ) )     // '`~' for US
            .add_key_handler( this, 0, VK_OEM_5,     boost::bind( &ReviewManager::handle_continue, this ) )     //  '\|' for US
            .add_key_handler( this, 0, VK_RIGHT,     boost::bind( &ReviewManager::handle_next, this ) )
            .add_key_handler( this, 0, VK_NEXT,      boost::bind( &ReviewManager::handle_next, this ) )
            .add_key_handler( this, 0, VK_UP,        boost::bind( &ReviewManager::handle_replay, this ) )
            .add_key_handler( this, 0, VK_LEFT,      boost::bind( &ReviewManager::handle_previous, this ) )
            .add_key_handler( this, 0, VK_PRIOR,     boost::bind( &ReviewManager::handle_previous, this ) )
            .add_key_handler( this, 0, VK_DELETE,    boost::bind( &ReviewManager::handle_disable, this ) )
            .add_key_handler( this, 0, VK_ESCAPE,    boost::bind( &ReviewManager::handle_exit, this ) )
            ;
    }

    IInput::instance().run();
}


void ReviewManager::start()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_start, v.first ) );
    }
}


void ReviewManager::handle_continue()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_continue, v.first ) );
    }
}


void ReviewManager::handle_next()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_next, v.first ) );
    }
}


void ReviewManager::handle_previous()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_previous, v.first ) );
    }
}


void ReviewManager::handle_replay()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_replay, v.first ) );
    }
}


void ReviewManager::handle_disable()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_disable, v.first ) );
    }
}


void ReviewManager::handle_jump( size_t distance )
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_jump, v.first, distance ) );
    }
}


void ReviewManager::handle_jump_back( size_t distance )
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_jump_back, v.first, distance ) );
    }
}


void ReviewManager::handle_exit()
{
    IHotKey::instance().clear();
    IInput::instance().clear();

    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_exit, v.first ) );
    }

    SetForegroundWindow( GetConsoleWindow() );
    IInputSender::instance().key( VK_ESCAPE );
}
