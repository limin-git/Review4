#include "stdafx.h"
#include "Wallpaper.h"
#include "IConfigurationFile.h"
#include "SystemUtility.h"
#include "AlgorithmUtility.h"
#include "FileSystemUtility.h"
#include "Utility.h"
#include "IInput.h"
#include "IHotKey.h"


Wallpaper::Wallpaper()
    : m_frequence( 2 ),
      m_count( 0 ),
      m_disable( false ),
      m_check_picture( true )
{
    po::options_description options;
    options.add_options()
        ( "wallpaper.disable", po::wvalue<std::wstring>(), "disable" )
        ( "wallpaper.path", po::wvalue<std::wstring>(), "picture directory path" )
        ( "wallpaper.recycle-path", po::wvalue<std::wstring>(), "recycle directory path" )
        ( "wallpaper.frequence", po::value<size_t>(), "change picture frequence" )
        ( "wallpaper.check-picture", po::wvalue<std::wstring>(), "check whether the file is a picture or not" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).add_observer(this).variables_map();

    if ( vm.count( "wallpaper.disable" ) )
    {
        if ( m_disable = ( L"true" == vm["wallpaper.disable"].as<std::wstring>() ) )
        {
            return;
        }
    }

    if ( vm.count( "wallpaper.path" ) )
    {
        m_directory = fs::system_complete( vm["wallpaper.path"].as<std::wstring>() );
    }

    if ( vm.count( "wallpaper.recycle-path" ) )
    {
        m_recycle_directory = fs::system_complete( vm["wallpaper.recycle-path"].as<std::wstring>() );

        if ( ! m_recycle_directory.empty() && ! exists( m_recycle_directory ) )
        {
            boost::system::error_code ec;
            if ( ! create_directories( m_recycle_directory, ec ) )
            {
                m_recycle_directory.clear();
            }
        }
    }

    if ( vm.count( "wallpaper.check-picture" ) )
    {
        m_check_picture = ( L"true" == vm["wallpaper.check-picture"].as<std::wstring>() );
    }
}


Wallpaper::~Wallpaper()
{
}


void Wallpaper::set_wallpaper()
{
    if ( m_disable || m_pictures.size() < 2 )
    {
        return;
    }

    m_current = m_pictures.begin();
    size_t rand = Utility::random( 0, m_pictures.size() - 1 );
    std::advance( m_current, rand );
    Utility::set_system_wallpaper( *m_current );
}


void Wallpaper::remove_current_picture()
{
    if ( ! m_disable )
    {
        if ( m_recycle_directory.empty() )
        {
            Utility::remove_file( *m_current );
        }
        else
        {
            Utility::rename_file( *m_current, m_recycle_directory / m_current->filename() );
        }

        m_pictures.erase( m_current );
        set_wallpaper();
    }
}


void Wallpaper::handle_start()
{
    if ( m_disable || m_directory.empty() || m_recycle_directory.empty() )
    {
        return;
    }

    m_pictures = ( m_check_picture ?
                   Utility::get_files_of_directory_if( m_directory, &Utility::is_picture, 10 ) :
                   Utility::get_files_of_directory( m_directory, 10 ) );

    boost::thread t( boost::bind( &Wallpaper::search_pictures_thread, this ) );

    if ( m_pictures.empty() )
    {
        return;
    }

    set_wallpaper();
    IInput::instance().add_key_handler( this, 0, 'Z', boost::bind( &Wallpaper::remove_current_picture, this ) );
    IHotKey::instance().register_handler( this, 0, 'Z', boost::bind( &Wallpaper::remove_current_picture, this ) );
}


void Wallpaper::handle_exit()
{
    if ( ! m_disable && ! m_directory.empty() )
    {
        m_disable = true;
        IHotKey::instance().unregister_handler( this );
        IInput::instance().remove_key_handler( this ).remove_mouse_handler( this );
        Utility::set_system_wallpaper( "C:\\Windows\\Web\\Wallpaper\\Theme1\\img1.jpg" );        
    }
}


void Wallpaper::handle_continue()
{
    if ( ! m_disable )
    {
        if ( 0 == ( ++m_count % m_frequence ) )
        {
            set_wallpaper();
        }
    }
}


void Wallpaper::handle_replay()
{
    m_count = 0;
}


void Wallpaper::handle_next()
{
    if ( ! m_disable )
    {
        set_wallpaper();
        m_count = 0;
    }
}


void Wallpaper::handle_previous()
{
    if ( ! m_disable )
    {
        set_wallpaper();
        m_count = 0;
    }
}


void Wallpaper::handle_jump( size_t distance )
{
    if ( ! m_disable )
    {
        set_wallpaper();
        m_count = 0;
    }
}


void Wallpaper::handle_jump_back( size_t distance )
{
    if ( ! m_disable )
    {
        set_wallpaper();
        m_count = 0;
    }
}


void Wallpaper::handle_disable()
{
    if ( ! m_disable )
    {
        set_wallpaper();
        m_count = 0;
    }
}


void Wallpaper::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<size_t>( "wallpaper.frequence", vm, old ) )
    {
        m_frequence = vm["wallpaper.frequence"].as<size_t>();
    }
}


void Wallpaper::search_pictures_thread()
{
    fs::path current_path = *m_current;
    std::list<fs::path> pictures =
        ( m_check_picture ?
          Utility::get_files_of_directory_if( m_directory, &Utility::is_picture ) :
          Utility::get_files_of_directory( m_directory ) );
    m_pictures.swap( pictures );
    std::list<fs::path>::iterator it = std::find( m_pictures.begin(), m_pictures.end(), current_path );

    if ( it != m_pictures.end() )
    {
        m_current = it;
    }
}
