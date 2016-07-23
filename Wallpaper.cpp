#include "stdafx.h"
#include "Wallpaper.h"
#include "IConfigurationFile.h"
#include "SystemUtility.h"
#include "AlgorithmUtility.h"
#include "FileSystemUtility.h"
#include "Utility.h"
#include "IInput.h"
#include "IHotKey.h"

#define wallpaper_disable           "wallpaper.disable"
#define wallpaper_path              "wallpaper.path"
#define wallpaper_recycle_path      "wallpaper.recycle-path"
#define wallpaper_frequence         "wallpaper.frequence"
#define wallpaper_check_picture     "wallpaper.check-picture"


Wallpaper::Wallpaper()
    : m_frequence( 2 ),
      m_count( 0 ),
      m_disable( false ),
      m_check_picture( true )
{
    po::options_description options;
    options.add_options()
        ( wallpaper_disable,        po::wvalue<std::wstring>(),     "disable" )
        ( wallpaper_path,           po::wvalue<std::wstring>(),     "picture directory path" )
        ( wallpaper_recycle_path,   po::wvalue<std::wstring>(),     "recycle directory path" )
        ( wallpaper_frequence,      po::value<size_t>(),            "change picture frequence" )
        ( wallpaper_check_picture,  po::wvalue<std::wstring>(),     "check whether the file is a picture or not" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).add_observer(this).variables_map();

    if ( vm.count( wallpaper_disable ) )
    {
        if ( m_disable = ( L"true" == vm[wallpaper_disable].as<std::wstring>() ) )
        {
            return;
        }
    }

    if ( vm.count( wallpaper_path ) )
    {
        m_directory = fs::system_complete( vm[wallpaper_path].as<std::wstring>() );
    }

    if ( vm.count( wallpaper_recycle_path ) )
    {
        m_recycle_directory = fs::system_complete( vm[wallpaper_recycle_path].as<std::wstring>() );

        if ( ! m_recycle_directory.empty() && ! exists( m_recycle_directory ) )
        {
            boost::system::error_code ec;
            if ( ! create_directories( m_recycle_directory, ec ) )
            {
                m_recycle_directory.clear();
            }
        }
    }

    if ( vm.count( wallpaper_check_picture ) )
    {
        m_check_picture = ( L"true" == vm[wallpaper_check_picture].as<std::wstring>() );
    }
}


Wallpaper::~Wallpaper()
{
    handle_exit();
}


void Wallpaper::handle_start()
{
    if ( m_disable || m_directory.empty() || m_recycle_directory.empty() )
    {
        return;
    }

    std::vector<fs::path> pictures = ( m_check_picture ?
                   Utility::get_files_of_directory_if_n( m_directory, &Utility::is_picture, 10 ) :
                   Utility::get_files_of_directory_n( m_directory, 10 ) );
    if ( pictures.empty() )
    {
        return;
    }

    Utility::random_shuffle( pictures );
    m_pictures.assign( pictures.begin(), pictures.end() );
    m_current = m_pictures.begin();
    Utility::set_system_wallpaper( *m_current );
    m_search_thread = boost::thread( boost::bind( &Wallpaper::search_pictures_thread, this ) );
    IInput::instance().register_handler( this, 0, 'Z', boost::bind( &Wallpaper::remove_current_picture, this ) );
    IHotKey::instance().register_handler( this, 0, 'Z', boost::bind( &Wallpaper::remove_current_picture, this ) );
}


void Wallpaper::handle_exit()
{
    m_search_thread.join();

    if ( ! m_disable && ! m_directory.empty() )
    {
        m_disable = true;
        //IHotKey::instance().unregister_handler( this );
        //IInput::instance().unregister_handler( this ).unregister_mouse_handler( this );
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


void Wallpaper::set_wallpaper()
{
    if (  ! m_disable )
    {
        boost::lock_guard<boost::recursive_mutex> lock( m_mutex );

        if ( ++m_current == m_pictures.end() )
        {
            m_current = m_pictures.begin();
        }

        Utility::set_system_wallpaper( *m_current );
    }
}


void Wallpaper::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<size_t>( wallpaper_frequence, vm, old ) )
    {
        m_frequence = vm[wallpaper_frequence].as<size_t>();
    }
}


void Wallpaper::remove_current_picture()
{
    if ( ! m_disable )
    {
        boost::lock_guard<boost::recursive_mutex> lock( m_mutex );

        if ( m_recycle_directory.empty() )
        {
            Utility::remove_file( *m_current );
        }
        else
        {
            Utility::rename_file( *m_current, m_recycle_directory / m_current->filename() );
        }

        m_pictures.erase( m_current++ );

        if ( m_current == m_pictures.end() )
        {
            m_current = m_pictures.begin();
        }

        Utility::set_system_wallpaper( *m_current );
    }
}


void Wallpaper::search_pictures_thread()
{
    std::vector<fs::path> pictures =
        ( m_check_picture ?
          Utility::get_files_of_directory_if( m_directory, &Utility::is_picture ):
          Utility::get_files_of_directory( m_directory ) );
    Utility::random_shuffle( pictures );
    boost::lock_guard<boost::recursive_mutex> lock( m_mutex );
    m_pictures.assign( pictures.begin(), pictures.end() );
    m_current = m_pictures.begin();
}
