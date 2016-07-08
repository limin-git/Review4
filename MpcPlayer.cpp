#include "stdafx.h"
#include "MpcPlayer.h"
#include "IConfigurationFile.h"
#include "IInputSender.h"
#include "InputUtility.h"
#include "IConsole.h"
#include "ILog.h"


MpcPlayer::MpcPlayer()
    : m_player_hwnd( NULL ),
      m_load_subtitle( false ),
      m_auto_stop( true ),
      m_playing( false )
{
    m_console = GetConsoleWindow();

    po::options_description options( "Movie" );
    options.add_options()
        ( "movie.player", po::wvalue<std::wstring>(), "mpc player path" )
        ( "movie.movie", po::wvalue<std::wstring>(), "the movie path" )
        ( "movie.load-subtitle", po::wvalue<std::wstring>(), "load subtitle?" )
        ( "movie.auto-stop", po::wvalue<std::wstring>(), "auto stop?" )
        ( "movie.wait-player-startup", po::value<size_t>()->default_value( 3 ), "wait the play to startup in seconds" )
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
        m_subtitle_path = vm["file.name"].as<std::wstring>();
        system_complete( m_subtitle_path );
    }

    if ( vm.count( "movie.load-subtitle" ) )
    {
        m_load_subtitle = ( L"true" == vm["movie.load-subtitle"].as<std::wstring>() );
    }

    if ( vm.count( "movie.auto-stop" ) )
    {
        m_auto_stop = ( L"true" == vm["movie.auto-stop"].as<std::wstring>() );
    }

    if ( vm.count( "movie.wait-player-startup" ) )
    {
        m_wait_startup = vm["movie.wait-player-startup"].as<size_t>();
    }

    initialize();
}


MpcPlayer::~MpcPlayer()
{
    close();
    m_processor.terminate();
}


bool MpcPlayer::play( ISubtitleSlideshowPtr subtitle )
{
    if ( m_player_hwnd == NULL )
    {
        return false;
    }

    SetForegroundWindow( m_player_hwnd );
    IInputSender::instance().Ctrl_key( 'G' );

    if ( ! hide_goto_dialog() )
    {
        return false;
    }

    std::stringstream ss;
    ss  << std::setfill('0')
        << std::setw(2) << subtitle->hour()
        << std::setw(2) << subtitle->minute()
        << std::setw(2) << subtitle->second()
        << std::setw(3) << subtitle->millisecond()
        ;
    IInputSender::instance().string( ss.str() ).key( VK_RETURN );
    m_subtitle = subtitle;
    m_processor.queue_item( m_subtitle );
    return true;
}


bool MpcPlayer::hide_goto_dialog( size_t timeout )
{
    for ( size_t i = 0; i < timeout; ++i )
    {
        HWND hwnd = FindWindow( L"#32770", L"×ªµ½..." );

        if ( hwnd != NULL )
        {
            ShowWindow( hwnd, SW_HIDE );
            return true;
        }

        Sleep( 1 );
    }

    return false;
}


void MpcPlayer::play_thread( const ISubtitleSlideshowPtr& subtitle )
{
    play();

    size_t duration = subtitle->duration();

    for ( size_t i = 0, step = 5; i < duration; i += step )
    {
        if ( m_subtitle != subtitle )
        {
            return;
        }

        Sleep( step );
    }

    pause();
}


void MpcPlayer::play()
{
    if ( ! m_playing )
    {
        SetForegroundWindow( m_player_hwnd );
        IInputSender::instance().key( VK_SPACE );
        m_playing = true;
    }
}


void MpcPlayer::pause()
{
    if ( m_playing )
    {
        SetForegroundWindow( m_player_hwnd );
        IInputSender::instance().key( VK_SPACE );
        m_playing = false;
        SetForegroundWindow( m_console );
    }
}


void MpcPlayer::close()
{
    if ( m_pi.hProcess )
    {
        TerminateProcess( m_pi.hProcess, 0 );
        CloseHandle( m_pi.hProcess );
        CloseHandle( m_pi.hThread );
    }
}


void MpcPlayer::initialize()
{
    if ( open_player() )
    {
        locate_player();
        m_processor.set_callback( boost::bind( &MpcPlayer::play_thread, this, _1 ) );
    }
}


bool MpcPlayer::open_player()
{
    ZeroMemory( &m_si, sizeof(m_si) );
    m_si.cb = sizeof(m_si);
    ZeroMemory( &m_pi, sizeof(m_pi) );

    if ( m_player.empty() || m_movie.empty() || m_subtitle_path.empty() )
    {
        return false;
    }

    std::wstringstream cmd_strm;
    cmd_strm << m_player << " " << m_movie << " /open";

    if ( m_load_subtitle )
    {
        cmd_strm << " /sub " << m_subtitle_path;
    }

    std::wstring cmd = cmd_strm.str();

    return CreateProcess( NULL, const_cast<wchar_t*>( cmd.c_str() ), 0, 0, 0, 0, 0, 0,&m_si,&m_pi ) != 0;
}


void MpcPlayer::locate_player()
{
    for ( size_t i = 0, step = 10; i < 5000; i += step )
    {
        m_player_hwnd = FindWindow( L"MediaPlayerClassicW", m_movie.filename().c_str() );

        if ( m_player_hwnd != NULL )
        {
            break;
        }

        Sleep( step );
    }

    if ( m_player_hwnd )
    {
        Sleep( m_wait_startup * 1000 );
    }
}
