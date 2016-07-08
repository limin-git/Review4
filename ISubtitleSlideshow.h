#pragma once
#include "ISlideShow.h"


struct ISubtitleSlideshow : ISlideshow
{
    virtual size_t hour() = 0;
    virtual size_t minute() = 0;
    virtual size_t second() = 0;
    virtual size_t millisecond() = 0;
    virtual size_t duration() = 0;
};

typedef boost::shared_ptr<ISubtitleSlideshow> ISubtitleSlideshowPtr;
typedef std::list<ISubtitleSlideshowPtr> ISubtitleSlideshowPtrList;
