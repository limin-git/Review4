#pragma once
#include "ISlideShow.h"


struct IScheduler
{
    virtual ~IScheduler() {}
    virtual ISlideshowPtr get_slideshow() = 0;
    static IScheduler& instance();
    static void remove();
};
