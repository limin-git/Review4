#pragma once
#include "ISpeech.h"
//#include <sapi.h>
#include <sapi.h>


struct Speech : ISpeech
{
public:

    Speech();
    ~Speech();
    virtual void speak( const std::wstring& word );
    virtual void speak( const std::vector<std::wstring>& words );

private:

    ISpVoice* m_sp_voice;
};
