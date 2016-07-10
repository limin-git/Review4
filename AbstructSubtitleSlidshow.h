#pragma once
#include "ISubtitleSlideshow.h"
#include "SubTime.h"
#include "IMoviePlayer.h"
#include "IConsole.h"
#include "Singleton.h"


struct AbstructSubtitleSlidshow : ISubtitleSlideshow
{
public:

    AbstructSubtitleSlidshow( size_t key, const SubTime& start_t, const SubTime& end_t, const std::wstring& txt, Singleton<IConsole>& console, Singleton<IMoviePlayer>& player );
    virtual void clear_state() {}
    virtual size_t key() { return m_key; }
    virtual const std::wstring& key_string() { return text; }
    virtual bool empty() { return false; }

    virtual size_t hour() { return start_time.hour; }
    virtual size_t minute() { return start_time.minute; }
    virtual size_t second() { return start_time.second; }
    virtual size_t millisecond() { return start_time.millisecond; }
    virtual size_t duration() { return end_time.milliseconds - start_time.milliseconds; }

public:

    size_t m_key;
    SubTime start_time;
    SubTime end_time;
    std::wstring text;
    Singleton<IConsole>& m_console;
    Singleton<IMoviePlayer>& m_player;
};
