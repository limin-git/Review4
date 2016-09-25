#pragma once
#include "ISlideShow.h"
#include "IDisableObserver.h"


struct IFilter
{
    virtual ~IFilter() {}
    virtual void filter( ISlideshowPtr slideshow ) = 0;
    virtual bool is_filtered( const std::wstring& str ) = 0;
    static IFilter& instance();
};
