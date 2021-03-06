#include "stdafx.h"
#include "MpcPlayer.h"
#include "IConfigurationFile.h"
#include "IInputSender.h"
#include "InputUtility.h"
#include "IConsole.h"
#include "Utility.h"

#define movie_disable                   "movie.disable"
#define movie_player                    "movie.player"
#define movie_movie                     "movie.movie"
#define movie_load_subtitle             "movie.load-subtitle"
#define movie_auto_stop                 "movie.auto-stop"
#define movie_wait_player_startup       "movie.wait-player-startup"
#define movie_adjust_start_time         "movie.adjust-start-time"
#define movie_adjust_duration_time      "movie.adjust-duration-time"
#define movie_more_than_an_hour         "movie.more-than-an-hour"


MpcPlayer::MpcPlayer()
    : m_player_hwnd( NULL ),
      m_load_subtitle( false ),
      m_auto_stop( true ),
      m_playing( false ),
      m_running( true ),
      m_adjust_start_time( 0 ),
      m_adjust_duration_time( 0 ),
      m_disable( false ),
      m_more_than_an_hour( true )
{
    m_console = GetConsoleWindow();

    po::options_description options( "Movie" );
    options.add_options()
        ( movie_disable,                po::wvalue<std::wstring>(),                 "disable" )
        ( movie_player,                 po::wvalue<std::wstring>(),                 "mpc player path" )
        ( movie_movie,                  po::wvalue<std::wstring>(),                 "the movie path" )
        ( movie_load_subtitle,          po::wvalue<std::wstring>(),                 "load subtitle?" )
        ( movie_auto_stop,              po::wvalue<std::wstring>(),                 "auto stop?" )
        ( movie_wait_player_startup,    po::value<size_t>()->default_value( 3 ),    "wait the play to startup in seconds" )
        ( movie_adjust_start_time,      po::value<int>()->default_value( 0 ),       "adjust subtitle display start time" )
        ( movie_adjust_duration_time,   po::value<int>()->default_value( 0 ),       "adjust subtitle display duration time" )
        ( movie_more_than_an_hour,      po::wvalue<std::wstring>(),                 "whether movie play time is more than an hour" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).add_observer(this).variables_map();

    if ( vm.count( movie_disable ) )
    {
        if ( m_disable = ( L"true" == vm[movie_disable].as<std::wstring>() ) )
        {
            return;
        }
    }

    if ( vm.count( "file.name" ) )
    {
        m_subtitle_path = fs::system_complete( vm["file.name"].as<std::wstring>() );
    }

    if ( vm.count( movie_player ) )
    {
        m_player = fs::system_complete( vm[movie_player].as<std::wstring>() );
    }

    if ( vm.count( movie_movie ) )
    {
        m_movie = fs::system_complete( vm[movie_movie].as<std::wstring>() );
    }

    if ( vm.count( movie_load_subtitle ) )
    {
        m_load_subtitle = ( L"true" == vm[movie_load_subtitle].as<std::wstring>() );
    }

    if ( vm.count( movie_wait_player_startup ) )
    {
        m_wait_startup = vm[movie_wait_player_startup].as<size_t>();
    }

    initialize();
}


MpcPlayer::~MpcPlayer()
{
    m_running = false;
    terminate_player();
    m_monitor_player_thread.join();
}


bool MpcPlayer::play( ISubtitleSlideshowPtr subtitle )
{
    if ( m_disable || NULL == m_player_hwnd )
    {
        return false;
    }

    boost::lock_guard<boost::mutex> lock( m_mutex );

    SetForegroundWindow( m_player_hwnd );
    IInputSender::instance().Ctrl_key( 'G' );

    if ( ! hide_goto_dialog() )
    {
        return false;
    }

    int sec = subtitle->second();
    int mil = subtitle->millisecond();
    int min = subtitle->minute();
    int hor = subtitle->hour();

    if ( m_adjust_start_time )
    {
        mil += m_adjust_start_time;
        while ( mil < 0 ) { mil += 1000; while ( --sec < 0 ) { sec += 60; while ( --min < 0 ) { min += 60; hor--; } } }
        while ( 1000 <= mil ) {  mil -= 1000; while ( 60 <= ++sec ) { sec -= 60; while ( 60 <= ++min ) { min -= 60; ++hor; } } }
    }

    std::stringstream ss;

    if ( m_more_than_an_hour )
    {
        ss << std::setw(2) << std::setfill('0') << hor;
    }

    ss  << std::setw(2) << std::setfill('0') << min
        << std::setw(2) << std::setfill('0') << sec
        << std::setw(2) << std::setfill('0') << mil
        ;
    IInputSender::instance().string( ss.str() ).key( VK_RETURN );
    m_subtitle = subtitle;
    m_condition.notify_one();
    m_processor.queue_item( boost::bind( &MpcPlayer::play_thread, this, m_subtitle ) );
    return true;
}


bool MpcPlayer::hide_goto_dialog( size_t timeout )
{
    for ( size_t i = 0, step = 1; i < timeout; i+= step )
    {
        HWND hwnd = FindWindow( L"#32770", L"ת��..." );

        if ( hwnd != NULL )
        {
            ShowWindow( hwnd, SW_HIDE );
            return true;
        }

        Sleep( step );
    }

    return false;
}


void MpcPlayer::play_thread( const ISubtitleSlideshowPtr& subtitle )
{
    play();

    size_t duration = subtitle->duration();

    if ( ( m_adjust_duration_time < 0 && static_cast<size_t>(- m_adjust_duration_time) < duration ) || 0 < m_adjust_duration_time )
    {
        duration += m_adjust_duration_time;
    }

    boost::unique_lock<boost::mutex> lock( m_mutex );
    boost::chrono::steady_clock::time_point end_time = boost::chrono::steady_clock::now() + boost::chrono::milliseconds( duration );

    while ( boost::chrono::steady_clock::now() < end_time )
    {
        boost::cv_status status = m_condition.wait_until( lock, end_time );

        if ( subtitle != m_subtitle )
        {
            return;
        }
        else if ( boost::cv_status::timeout == status )
        {
            break;
        }
    }

    if ( m_auto_stop )
    {
        pause();
    }
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
    }
}


void MpcPlayer::terminate_player()
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
    BOOL result = CreateProcess( NULL, const_cast<wchar_t*>( cmd.c_str() ), 0, 0, 0, 0, 0, 0, &m_si, &m_pi ) != 0;

    if ( result )
    {
        m_monitor_player_thread = boost::thread( boost::bind( &MpcPlayer::monitor_player_process_thread, this ) );
    }

    return ( result != FALSE );
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


void MpcPlayer::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<int>( movie_adjust_start_time, vm, old ) )
    {
        m_adjust_start_time = vm[movie_adjust_start_time].as<int>();
    }

    if ( Utility::updated<int>( movie_adjust_duration_time, vm, old ) )
    {
        m_adjust_duration_time = vm[movie_adjust_duration_time].as<int>();
    }

    if ( vm.count( movie_auto_stop ) )
    {
        m_auto_stop = ( L"true" == vm[movie_auto_stop].as<std::wstring>() );
    }

    if ( vm.count( movie_more_than_an_hour ) )
    {
        m_more_than_an_hour = ( L"true" == vm[movie_more_than_an_hour].as<std::wstring>() );
    }
}


void MpcPlayer::monitor_player_process_thread()
{
    WaitForSingleObject( m_pi.hProcess, INFINITE );
    ZeroMemory( &m_si, sizeof(m_si) );
    m_si.cb = sizeof(m_si);
    ZeroMemory( &m_pi, sizeof(m_pi) );
    m_player_hwnd = NULL;
}
