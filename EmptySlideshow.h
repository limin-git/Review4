#pragma once
#include "ISlideShow.h"


struct EmptySlideshow : ISlideshow
{
public:

    EmptySlideshow();
    virtual bool show();
    virtual size_t key();
    virtual const std::wstring& key_string();
};
