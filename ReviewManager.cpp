#include "stdafx.h"
#include "ReviewManager.h"
#include "IInput.h"
#include "IHotKey.h"
#include "IInputSender.h"
#include "IConfigurationFile.h"
#include "TextReview.h"
#include "SubtitleReview.h"
#include "Wallpaper.h"
#include "Utility.h"
#include "ILog.h"
namespace po = boost::program_options;
namespace fs = boost::filesystem;

#define advanced_register_hot_keys                  "advanced.register-hot-keys"
#define advanced_enable_key_asdw                    "advanced.enable-key-asdw"
#define advanced_disable_hotkeys_when_listening     "advanced.disable-hotkeys-when-listening"
#define file_name_option                            "file.name"
#define wallpaper_path                              "wallpaper.path"


ReviewManager::ReviewManager()
    : m_register_hot_keys( false ),
      m_disable_hotkeys_when_listening( true ),
      m_listening( false )
{
    po::options_description options( "" );
    options.add_options()
        ( advanced_register_hot_keys,               po::wvalue<std::wstring>(), "register hot keys?" )
        ( advanced_enable_key_asdw,                 po::wvalue<std::wstring>(), "enable shortcut key ASDW?" )
        ( advanced_disable_hotkeys_when_listening,  po::wvalue<std::wstring>(), "disable hotkeys when listening(default: true)?" )
        ( file_name_option,                         po::wvalue<std::wstring>(), "file name" )
        ( wallpaper_path,                           po::wvalue<std::wstring>(), "register hot keys?" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).add_observer( this ).variables_map();

    if ( vm.count( advanced_register_hot_keys ) )
    {
        m_register_hot_keys = ( L"true" == vm[advanced_register_hot_keys].as<std::wstring>() );
    }

    if ( vm.count( file_name_option ) )
    {
        fs::path file_name = fs::system_complete( vm[file_name_option].as<std::wstring>() );

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

    if ( vm.count( wallpaper_path ) )
    {
        m_reivews[new Wallpaper] = new QueueProcessor<1>;
    }
}


ReviewManager::~ReviewManager()
{
    //IInput::instance().unregister_handler( this ).unregister_mouse_handler( this );
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

    IHotKey::instance()
        .register_handler( this, MOD_CONTROL|MOD_ALT, 'L',  boost::bind( &ReviewManager::handle_listen, this ) )
        ;

    regist_hot_keys();

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

    IInput::instance().terminate();
    SetForegroundWindow( GetConsoleWindow() );
    IInputSender::instance().key( VK_ESCAPE );
}


void ReviewManager::handle_review_again()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_review_again, v.first ) );
    }
}


void ReviewManager::handle_listen()
{
    m_listening = !m_listening;

    unregister_hot_keys();

    if ( m_listening )
    {
        IHotKey::instance().register_handler( this, MOD_CONTROL, 'L', boost::bind( &ReviewManager::handle_listen, this ) );
    }
    else
    {
        regist_hot_keys();
    }

    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_listen, v.first ) );
    }
}


void ReviewManager::handle_filter()
{
    BOOST_FOREACH( ReviewMap::value_type& v, m_reivews )
    {
        v.second->queue_item( boost::bind( &IReview::handle_filter, v.first ) );
    }
}


void ReviewManager::regist_hot_keys()
{
    log_info << L"ReviewManager::regist_hot_keys";

    IHotKey::instance()
        .register_handler( this, 0, VK_DOWN,                boost::bind( &ReviewManager::handle_continue, this ) )
        //.register_handler( this, 0, VK_OEM_3,             boost::bind( &ReviewManager::handle_continue, this ) )     // '`~' for US
        //.register_handler( this, 0, VK_OEM_5,             boost::bind( &ReviewManager::handle_continue, this ) )     //  '\|' for US
        .register_handler( this, 0, VK_LEFT,                boost::bind( &ReviewManager::handle_previous, this ) )
        .register_handler( this, 0, VK_PRIOR,               boost::bind( &ReviewManager::handle_previous, this ) )
        .register_handler( this, 0, VK_BROWSER_BACK,        boost::bind( &ReviewManager::handle_previous, this ) )
        .register_handler( this, 0, VK_RIGHT,               boost::bind( &ReviewManager::handle_next, this ) )
        .register_handler( this, 0, VK_NEXT,                boost::bind( &ReviewManager::handle_next, this ) )
        .register_handler( this, 0, VK_BROWSER_FORWARD,     boost::bind( &ReviewManager::handle_next, this ) )
        .register_handler( this, 0, VK_UP,                  boost::bind( &ReviewManager::handle_replay, this ) )
        .register_handler( this, 0, VK_DELETE,              boost::bind( &ReviewManager::handle_disable, this ) )
        .register_handler( this, 0, VK_ESCAPE,              boost::bind( &ReviewManager::handle_exit, this ) )
        //.register_handler( this, MOD_CONTROL, VK_RIGHT,   boost::bind( &ReviewManager::handle_jump, this, 10 ) )
        //.register_handler( this, MOD_CONTROL, VK_DOWN,    boost::bind( &ReviewManager::handle_jump, this, 50 ) )
        //.register_handler( this, MOD_CONTROL, VK_NEXT,    boost::bind( &ReviewManager::handle_jump, this, 100 ) )
        //.register_handler( this, MOD_CONTROL, VK_LEFT,    boost::bind( &ReviewManager::handle_jump_back, this, 10 ) )
        //.register_handler( this, MOD_CONTROL, VK_UP,      boost::bind( &ReviewManager::handle_jump_back, this, 50 ) )
        //.register_handler( this, MOD_CONTROL, VK_PRIOR,   boost::bind( &ReviewManager::handle_jump_back, this, 100 ) )
        .register_handler( this, 0, 'R',                    boost::bind( &ReviewManager::handle_review_again, this ) )
        .register_handler( this, MOD_CONTROL, 'L',          boost::bind( &ReviewManager::handle_listen, this ) )
        .register_handler( this, MOD_CONTROL, VK_DELETE,    boost::bind( &ReviewManager::handle_filter, this ) )
        ;

    po::variables_map& vm = IConfigurationFile::instance().variables_map();

    if ( vm.count( advanced_enable_key_asdw ) && vm[advanced_enable_key_asdw].as<std::wstring>() == L"true" )
    {
        IHotKey::instance()
            .register_handler( this, 0, 'S',                boost::bind( &ReviewManager::handle_continue, this ) )
            .register_handler( this, 0, 'A',                boost::bind( &ReviewManager::handle_previous, this ) )
            .register_handler( this, 0, 'D',                boost::bind( &ReviewManager::handle_next, this ) )
            .register_handler( this, 0, 'W',                boost::bind( &ReviewManager::handle_replay, this ) )
            ;
    }
}


void ReviewManager::unregister_hot_keys()
{
    log_info << L"ReviewManager::unregister_hot_keys";

    IHotKey::instance().unregister_handler( this );
}


void ReviewManager::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<std::wstring>( advanced_disable_hotkeys_when_listening, vm, old ) )
    {
        m_disable_hotkeys_when_listening = ( L"true" == vm[advanced_disable_hotkeys_when_listening].as<std::wstring>() );
    }
}
