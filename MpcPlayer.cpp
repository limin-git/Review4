#include "stdafx.h"
#include "MpcPlayer.h"
#include "IConfigurationFile.h"
#include "InputUtility.h"


MpcPlayer::MpcPlayer()
    : m_hwnd( NULL )
{
    po::options_description options( "Movie" );
    options.add_options()
        ( "movie.player", po::wvalue<std::wstring>(), "mpc player path" )
        ( "movie.movie", po::wvalue<std::wstring>(), "the movie path" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).variables_map();
    
    if ( vm.count( "movie.player" ) )
    {
        m_player = vm["movie.player"].as<std::wstring>();
        system_complete( m_player );
    }

    if ( vm.count( "movie.movie" ) )
    {
        m_movie = vm["movie.movie"].as<std::wstring>();
        system_complete( m_movie );
    }

    if ( vm.count( "file.name" ) )
    {
        m_subtitle = vm["file.name"].as<std::wstring>();
        system_complete( m_subtitle );
    }

    open();
}


MpcPlayer::~MpcPlayer()
{
    close();
}


bool MpcPlayer::go_to( size_t hour, size_t minute, size_t second, size_t millisecond, size_t duration )
{
    if ( m_hwnd == NULL )
    {
        return false;
    }

    SetForegroundWindow( m_hwnd );
    Utility::send_input_ctrl( 'G' );

    {
        std::wstring class_name = L"#32770";
        std::wstring window_name = L"×ªµ½...";
        HWND hwnd = NULL;

        for ( size_t i = 0; i < 200; ++i )
        {
            hwnd = FindWindow( class_name.c_str(), window_name.c_str() );

            if ( hwnd != NULL )
            { 
                ShowWindow( hwnd, SW_HIDE );
                break;
            }

            Sleep( 1 );
        }

        if ( hwnd == NULL )
        {
            return false;
        }
    }

    std::stringstream strm;
    strm << std::setw(2) << std::setfill('0') << hour;
    strm << std::setw(2) << std::setfill('0') << minute;
    strm << std::setw(2) << std::setfill('0') << second;
    strm << std::setw(3) << std::setfill('0') << millisecond;

    Utility::send_input( strm.str() );
    Utility::send_input_enter();
    Utility::send_input( VK_SPACE );
    Sleep( duration );
    Utility::send_input( VK_SPACE );
    SetForegroundWindow( GetConsoleWindow() );
    return true;
}


void MpcPlayer::pause()
{
    Utility::send_input( VK_SPACE );
}


void MpcPlayer::open()
{
    if ( m_player.empty() || m_movie.empty() || m_subtitle.empty() )
    {
        return;
    }

    std::wstringstream parameter;
    parameter << "\"" << m_movie << "\" /open /sub \"" << m_subtitle << "\"";

    ShellExecute(
        NULL,
        L"open",
        m_player.c_str(),
        parameter.str().c_str(),
        0,
        SW_SHOWNOACTIVATE );

    const std::wstring class_name = L"MediaPlayerClassicW";

    for ( size_t i = 0; i < 500; ++i )
    {
        m_hwnd = FindWindow( class_name.c_str(), m_movie.filename().wstring().c_str() );

        if ( m_hwnd != NULL )
        {
            break;
        }

        Sleep( 10 );
    }
}


void MpcPlayer::close()
{
    // TODO: run as administrator
    //system( "taskkill /im mpc-hc64.exe" );
}
