#pragma once
#include "ISubtitleSlideshow.h"
#include "StartEndTime.h"


struct AssSlideshow : ISubtitleSlideshow
{
public:

    AssSlideshow( size_t key, const StartEndTime& start_t, const StartEndTime& end_t, const std::wstring& txt );
    virtual bool show();
    virtual void clear_state();
    virtual size_t key();
    virtual const std::wstring& key_string();
    virtual bool empty() { return false; }

    virtual size_t hour() { return start_time.hour; }
    virtual size_t minute() { return start_time.minute; }
    virtual size_t second() { return start_time.second; }
    virtual size_t millisecond() { return start_time.millisecond; }
    virtual size_t duration() { return end_time.milliseconds - start_time.milliseconds; }

public:

    size_t m_key;
    StartEndTime start_time;
    StartEndTime end_time;
    std::wstring text;
};
