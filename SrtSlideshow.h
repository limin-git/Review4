#pragma once
#include "ISlideShow.h"
#include "StartEndTime.h"


struct SrtSlideshow : ISlideshow
{
public:

    SrtSlideshow( size_t key, size_t num, StartEndTime start_t, StartEndTime end_t, const std::wstring& txt, const std::wstring& txt2 );
    virtual bool show();
    virtual void clear_state();
    virtual size_t key();
    virtual const std::wstring& key_string();
    virtual bool empty() { return false; }

public:

    size_t m_key;
    size_t number;
    StartEndTime start_time;
    StartEndTime end_time;
    std::wstring text;
    std::wstring text2;
};
