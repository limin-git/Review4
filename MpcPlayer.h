#pragma once
#include "IMoviePlayer.h"
#include "QueueProcessor.h"
namespace fs = boost::filesystem;


struct MpcPlayer : IMoviePlayer
{
public:

    MpcPlayer();
    ~MpcPlayer();
    virtual bool play( ISubtitleSlideshowPtr subtitle );

private:

    void initialize();
    bool open_player();
    void locate_player();
    void close();
    void play();
    void pause();
    void play_thread( const ISubtitleSlideshowPtr& info );
    bool hide_goto_dialog( size_t timeout = 200 );

private:
    
    fs::path m_player;
    fs::path m_movie;
    fs::path m_subtitle_path;
    HWND m_player_hwnd;
    HWND m_console;
    STARTUPINFO m_si;
    PROCESS_INFORMATION m_pi;
    QueueProcessor<ISubtitleSlideshowPtr> m_processor;
    bool m_load_subtitle;
    bool m_auto_stop;
    size_t m_wait_startup;
    volatile bool m_playing;
    ISubtitleSlideshowPtr m_subtitle;
};
