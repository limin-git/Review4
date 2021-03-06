#pragma once
#include "ISlideShow.h"
#include "IDisableObserver.h"


struct IDisable
{
    virtual ~IDisable() {}
    virtual void disable( ISlideshowPtr slideshow ) = 0;
    virtual const std::set<std::wstring>& disabled() = 0;
    virtual bool is_disabled( size_t key ) = 0;
    virtual bool is_disabled( const std::wstring& key ) = 0;
    virtual void add_observer( IDisableObserver* observer ) = 0;
    virtual void remove_observer( IDisableObserver* observer ) = 0;
    static IDisable& instance();
};
