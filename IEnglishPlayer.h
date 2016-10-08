#pragma once
#include "Singleton.h"


struct IEnglishPlayer
{
    virtual ~IEnglishPlayer() {};
    virtual void speak( const std::wstring& word ) = 0;
    virtual void speak( const std::vector<std::wstring>& words ) = 0;
    virtual bool is_synchronized() = 0;
    virtual void synchronize( bool is_sync ) = 0;
    static IEnglishPlayer& instance();
};
