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

    void open();
    void close();

private:
    
    fs::path m_player;
    fs::path m_movie;
    fs::path m_subtitle;
    HWND m_hwnd;
};
