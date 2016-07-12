#pragma once
#include "ISubtitleSlideshow.h"


struct IMoviePlayer
{
    virtual ~IMoviePlayer() {}
    virtual bool play( ISubtitleSlideshowPtr subtitle ) = 0;
    static IMoviePlayer& instance();
};
