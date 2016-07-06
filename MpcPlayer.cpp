#include "stdafx.h"
#include "MpcPlayer.h"
#include "IConfigurationFile.h"
#include "InputUtility.h"


MpcPlayer::MpcPlayer()
    : m_hwnd( NULL ),
      m_load_subtitle( false ),
      m_auto_stop( true )
{
    po::options_description options( "Movie" );
    options.add_options()
        ( "movie.player", po::wvalue<std::wstring>(), "mpc player path" )
        ( "movie.movie", po::wvalue<std::wstring>(), "the movie path" )
        ( "movie.load-subtitle", po::wvalue<std::wstring>(), "load subtitle?" )
        ( "movie.auto-stop", po::wvalue<std::wstring>(), "auto stop?" )
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

    if ( vm.count( "movie.load-subtitle" ) )
    {
        m_load_subtitle = ( L"true" == vm["movie.load-subtitle"].as<std::wstring>() );
    }

    if ( vm.count( "movie.auto-stop" ) )
    {
        m_auto_stop = ( L"true" == vm["movie.auto-stop"].as<std::wstring>() );
    }

    initialize();
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

    if ( m_auto_stop )
    {
        Utility::send_input( VK_SPACE );
        Sleep( duration );
        Utility::send_input( VK_SPACE );
        SetForegroundWindow( GetConsoleWindow() );
    }

    return true;
}


void MpcPlayer::pause()
{
    Utility::send_input( VK_SPACE );
}


void MpcPlayer::close()
{
    TerminateProcess( m_pi.hProcess, 0 );
    CloseHandle( m_pi.hProcess );
    CloseHandle( m_pi.hThread );
}


void MpcPlayer::initialize()
{
    if ( open_player() )
    {
        locate_player();
    }
}


bool MpcPlayer::open_player()
{
    if ( m_player.empty() || m_movie.empty() || m_subtitle.empty() )
    {
        return false;
    }

    std::wstringstream cmd_strm;
    cmd_strm << m_player << " " << m_movie << " /open";

    if ( m_load_subtitle )
    {
        cmd_strm << " /sub " << m_subtitle;
    }

    std::wstring cmd = cmd_strm.str();

    ZeroMemory( &m_si, sizeof(m_si) );
    m_si.cb = sizeof(m_si);
    ZeroMemory( &m_pi, sizeof(m_pi) );

    return CreateProcess( NULL, const_cast<wchar_t*>( cmd.c_str() ), 0, 0, 0, 0, 0, 0,&m_si,&m_pi ) != 0;
}


void MpcPlayer::locate_player()
{
    for ( size_t i = 0; i < 500; ++i )
    {
        m_hwnd = FindWindow( L"MediaPlayerClassicW", m_movie.filename().c_str() );

        if ( m_hwnd != NULL )
        {
            break;
        }

        Sleep( 10 );
    }

    if ( m_hwnd )
    {
        Sleep( 3000 );
    }
}
