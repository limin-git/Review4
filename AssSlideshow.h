#pragma once
#include "AbstructSubtitleSlidshow.h"


struct AssSlideshow : AbstructSubtitleSlidshow
{
public:

    AssSlideshow( size_t key, const SubTime& start_t, const SubTime& end_t, const std::wstring& txt, Singleton<IConsole>& console, Singleton<IMoviePlayer>& player );
    virtual bool show();
};
