#pragma once
#include "IMoviePlayer.h"
#include "QueueProcessor.h"
#include "IConfigurationFileObserver.h"
namespace fs = boost::filesystem;


struct MpcPlayer : IMoviePlayer,
                   IConfigurationFileObserver
{
public:

    MpcPlayer();
    ~MpcPlayer();
    virtual bool play( ISubtitleSlideshowPtr subtitle );

public:

    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:

    void initialize();
    bool open_player();
    void locate_player();
    void terminate_player();
    void play();
    void pause();
    void play_thread( const ISubtitleSlideshowPtr& info );
    bool hide_goto_dialog( size_t timeout = 5000 );
    void monitor_player_process_thread();

private:

    fs::path m_player;
    fs::path m_movie;
    fs::path m_subtitle_path;
    HWND m_player_hwnd;
    HWND m_console;
    STARTUPINFO m_si;
    PROCESS_INFORMATION m_pi;
    QueueProcessor m_processor;
    bool m_load_subtitle;
    bool m_auto_stop;
    size_t m_wait_startup;
    int m_adjust_start_time;
    int m_adjust_duration_time;
    volatile bool m_playing;
    volatile bool m_running;
    ISubtitleSlideshowPtr m_subtitle;
    boost::thread m_monitor_player_thread;
};
