#pragma once
#include "ISlideShow.h"


struct ISchedule
{
    virtual ~ISchedule() {}
    virtual ISlideshowPtr next_slideshow() = 0;
    static ISchedule& instance();
    static void remove();
};
