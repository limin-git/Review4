#pragma once
#include "ISlideShow.h"


struct EmptySlideshow : ISlideshow
{
public:

    EmptySlideshow( bool finished = false );
    virtual bool show();
    virtual void clear_state() {};
    virtual size_t key();
    virtual const std::wstring& key_string();
    virtual bool empty() { return true; }

private:

    bool m_finished;
};
