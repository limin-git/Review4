#pragma once
#include "IMoviePlayer.h"
namespace fs = boost::filesystem;


struct MpcPlayer : IMoviePlayer
{
public:

    MpcPlayer();
    ~MpcPlayer();
    virtual bool go_to( size_t hour, size_t minute, size_t second, size_t millisecond, size_t duration );
    virtual void pause();

private:

    void close();
    void initialize();
    bool open_player();
    void locate_player();

private:
    
    fs::path m_player;
    fs::path m_movie;
    fs::path m_subtitle;
    bool m_load_subtitle;
    bool m_auto_stop;
    HWND m_hwnd;
    STARTUPINFO m_si;
    PROCESS_INFORMATION m_pi;
};
