#include "stdafx.h"
#include "Speech.h"
#include <sphelper.h>


Speech::Speech()
    : m_sp_voice( NULL )
{
    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
    ::CoCreateInstance( CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_sp_voice );

#if 1
    ISpObjectToken* token = NULL;
    //HRESULT find_token =  SpFindBestToken( SPCAT_VOICES, L"", L"Name=VW Julie", &token );
    //HRESULT find_token =  SpFindBestToken( L"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\SPEECH\\Voices", L"Vendor=Voiceware;Name=VW Paul", L"", &token );
    HRESULT find_token =  SpFindBestToken( L"HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\SPEECH\\Voices", L"Vendor=Microsoft;Name=Microsoft Tracy Desktop", L"", &token );

    if ( SUCCEEDED(find_token) )
    {
        HRESULT set_voice = m_sp_voice->SetVoice( token );

        if ( SUCCEEDED(set_voice) )
        {
            m_sp_voice->Speak( L"ÄãºÃÊÀ½çhello, world", 0, NULL );
        }
    }
#endif
}


Speech::~Speech()
{
    m_sp_voice->Release();
    ::CoUninitialize();
}


void Speech::speak( const std::wstring& word )
{
    if ( m_sp_voice )
    {
        m_sp_voice->Speak( word.c_str(), 0, NULL );
    }
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
