#include "stdafx.h"
#include "Wallpaper.h"
#include "IConfigurationFile.h"
#include "SystemUtility.h"
#include "AlgorithmUtility.h"
#include "FileSystemUtility.h"
#include "IInput.h"

namespace po = boost::program_options;


Wallpaper::Wallpaper()
{
    po::options_description options;
    options.add_options()
        ( "wallpaper.path", po::wvalue<std::wstring>(), "picture directory path" )
        ( "wallpaper.recycle-path", po::wvalue<std::wstring>(), "recycle directory path" )
        ;
    po::variables_map& vm = IConfigurationFile::instance()
        .add_options_description( options )
        .variables_map()
        ;

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

    m_pictures = Utility::get_files_of_directory( m_directory );
}


Wallpaper::~Wallpaper()
{
    if ( !m_directory.empty() )
    {
        IInput::instance()
            .remove_key_handler( this )
            .remove_mouse_handler( this )
            ;
        Utility::set_system_wallpaper( L"C:\\Windows\\Web\\Wallpaper\\Theme1\\img1.jpg" );
    }
}


void Wallpaper::run()
{
    if ( m_directory.empty() )
    {
        return;
    }

    set_wallpaper();

    IInput::instance()
        .add_key_handler( this, true, 'Z', 'Z',                     boost::bind( &Wallpaper::remove_current_picture, this ) )
        .add_key_handler( this, true, VK_LEFT, VK_DOWN,             boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_key_handler( this, true, VK_BACK,                      boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_key_handler( this, true, VK_OEM_3,                     boost::bind( &Wallpaper::set_wallpaper, this ) )     // '`~' for US
        .add_key_handler( this, true, VK_OEM_5,                     boost::bind( &Wallpaper::set_wallpaper, this ) )     //  '\|' for US
        .add_key_handler( this, true, VK_NEXT,                      boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_key_handler( this, true, VK_PRIOR,                     boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_key_handler( this, true, VK_DELETE,                    boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_mouse_handler( this, 0, FROM_LEFT_1ST_BUTTON_PRESSED,  boost::bind( &Wallpaper::set_wallpaper, this ) )
        .add_mouse_handler( this, 0, RIGHTMOST_BUTTON_PRESSED,      boost::bind( &Wallpaper::set_wallpaper, this ) )
        ;
}


void Wallpaper::search_pictures()
{
    fs::recursive_directory_iterator it( m_directory );
    fs::recursive_directory_iterator end;

    for ( ; it != end; ++it )
    {
        if ( ! is_directory( it.status() )  )
        {
            if ( Utility::is_picture( it->path() ) )
            {
                m_pictures.push_back( it->path() );
            }
        }
    }

    m_current = m_pictures.end();
}


void Wallpaper::set_wallpaper()
{
    if ( m_pictures.size() < 2 )
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
    if ( m_current != m_pictures.end() )
    {
        fs::path p = *m_current;

        if ( m_recycle_directory.empty() )
        {
            Utility::remove_file( *m_current );
        }
        else
        {
            Utility::rename_file( *m_current, m_recycle_directory / p.filename() );
        }

        m_pictures.erase( m_current );
        set_wallpaper();
    }
}
