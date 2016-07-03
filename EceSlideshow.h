#pragma once
#include "ISlideShow.h"


struct EceSlideshow : ISlideshow
{
public:

    EceSlideshow( size_t key, const std::wstring& eng, const std::wstring& chs, const std::wstring& exp );
    virtual bool show();
    virtual size_t key();
    virtual const std::wstring& key_string();

private:

    enum EStage{ English, Chinese, Example };

private:

    EStage m_stage;
    size_t m_key;
    std::wstring m_english;
    std::wstring m_chinese;
    std::wstring m_example;
};