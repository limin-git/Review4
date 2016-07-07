#pragma once


struct IMoviePlayer
{
    virtual ~IMoviePlayer() {}
    virtual bool go_to( size_t hour, size_t minute, size_t second, size_t millisecond, size_t duration ) = 0;
    static IMoviePlayer& instance();
    static void remove();
};
