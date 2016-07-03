#pragma once
#include "ISlideShow.h"


struct EceSlideshow : ISlideshow
{
public:

    EceSlideshow( const std::wstring& eng, const std::wstring& chs, const std::wstring& exp );
    virtual bool show();

private:

    enum EStage{ English, Chinese, Example };

private:

    EStage m_stage;
    std::wstring m_english;
    std::wstring m_chinese;
    std::wstring m_example;
};
