#pragma once
#include "AbstructSubtitleSlidshow.h"


struct SrtSlideshow : AbstructSubtitleSlidshow
{
public:

    SrtSlideshow( size_t key, size_t num, SubTime start_t, SubTime end_t, const std::wstring& txt, const std::wstring& txt2 );
    virtual bool show();

private:

    std::wstring text2;
};
