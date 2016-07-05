#pragma once
#include "ISlideShow.h"


struct Time
{
    size_t hour;
    size_t minute;
    size_t second;
    size_t millisecond;
    size_t total_millisecond;
};


struct SrtSubtitle : ISlideshow
{
public:

    SrtSubtitle( size_t key, size_t num, Time start_t, Time stop_t, const std::wstring& txt, const std::wstring& txt2 );
    virtual bool show();
    virtual void clear_state();
    virtual size_t key();
    virtual const std::wstring& key_string();
    virtual bool empty() { return false; }

public:

    size_t m_key;
    size_t number;
    Time start_time;
    Time stop_time;
    std::wstring text;
    std::wstring text2;
};
