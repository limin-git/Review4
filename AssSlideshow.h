#pragma once
#pragma once
#include "ISlideShow.h"
#include "StartEndTime.h"


struct AssSlideshow : ISlideshow
{
public:

    AssSlideshow( size_t key, const StartEndTime& start_t, const StartEndTime& end_t, const std::wstring& txt );
    virtual bool show();
    virtual void clear_state();
    virtual size_t key();
    virtual const std::wstring& key_string();
    virtual bool empty() { return false; }

public:

    size_t m_key;
    StartEndTime start_time;
    StartEndTime end_time;
    std::wstring text;
};
