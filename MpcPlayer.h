#pragma once
#include "IMoviePlayer.h"
#include "QueueProcessor.h"
namespace fs = boost::filesystem;


struct MpcPlayer : IMoviePlayer
{
public:

    MpcPlayer();
    ~MpcPlayer();
    virtual bool go_to( size_t hour, size_t minute, size_t second, size_t millisecond, size_t duration );

private:

    void close();
    void initialize();
    bool open_player();
    void locate_player();

public:

    struct GotoInfo
    {
        size_t hour;
        size_t minute;
        size_t second;
        size_t millisecond;
        size_t duration;
    };

    void go_to_thread( const GotoInfo& info );

private:
    
    fs::path m_player;
    fs::path m_movie;
    fs::path m_subtitle;
    bool m_load_subtitle;
    bool m_auto_stop;
    HWND m_hwnd;
    HWND m_console;
    STARTUPINFO m_si;
    PROCESS_INFORMATION m_pi;
    QueueProcessor<GotoInfo> m_processor;
};
