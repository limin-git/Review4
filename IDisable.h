#pragma once
#include "ISlideShow.h"


struct IDisable
{
    virtual ~IDisable() {}
    virtual void disable( ISlideshowPtr slideshow ) = 0;
    virtual const std::set<size_t>& disabled() = 0;
    static IDisable& instance();
    static void remove();
};
