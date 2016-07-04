#pragma once
#include "ISlideshow.h"
#include "ITextObserver.h"


struct IText
{
    virtual ~IText() {}
    virtual ISlideshowPtr slideshow( size_t hash_key ) = 0;
    virtual const std::set<size_t>& keys() = 0;
    virtual void add_observer( ITextObserver* observer ) = 0;
    virtual void remove_observer( ITextObserver* observer ) = 0;
    static IText& instance();
    static void remove();
};
