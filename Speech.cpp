#include "stdafx.h"
#include "Speech.h"


Speech::Speech()
{
    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
    ::CoCreateInstance( CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_sp_voice );
}


Speech::~Speech()
{
    m_sp_voice->Release();
    ::CoUninitialize();
}


void Speech::speak( const std::wstring& word )
{
    m_sp_voice->Speak( word.c_str(), 0, NULL );
}


void Speech::speak( const std::vector<std::wstring>& words )
{
    for ( size_t i = 0; i < words.size(); ++i )
    {
        m_sp_voice->Speak( words[i].c_str(), 0, NULL );

        if ( i + 1 < words.size() )
        {
            boost::this_thread::sleep_for( boost::chrono::milliseconds(300) );
        }
    }
}
