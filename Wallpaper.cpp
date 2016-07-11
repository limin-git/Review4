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
    po::variables_map& vm = m_configuration->add_options_description( options ).variables_map();

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
}


Wallpaper::~Wallpaper()
{
    if ( !m_directory.empty() )
    {
        m_input->remove_key_handler( this ).remove_mouse_handler( this );
        Utility::set_system_wallpaper( L"C:\\Windows\\Web\\Wallpaper\\Theme1\\img1.jpg" );
    }
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


void Wallpaper:: handle_start()
{
    m_pictures = Utility::get_files_of_directory( m_directory );

    if ( m_directory.empty() )
    {
        return;
    }

    set_wallpaper();
    m_input->add_key_handler( this, 0, 'Z', boost::bind( &Wallpaper::remove_current_picture, this ) );
}


void Wallpaper:: handle_continue()
{
}


void Wallpaper:: handle_replay()
{
}


void Wallpaper:: handle_next()
{
    set_wallpaper();
}


void Wallpaper:: handle_previous()
{
    set_wallpaper();
}


void Wallpaper:: handle_jump( size_t distance )
{
    set_wallpaper();
}


void Wallpaper:: handle_jump_back( size_t distance )
{
    set_wallpaper();
}


void Wallpaper:: handle_disable()
{
}
