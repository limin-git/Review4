#include "stdafx.h"
#include "IConfigurationFile.h"
#include "Utility.h"
#include "Speech.h"
#include <sphelper.h>

#define speech_name "speech.name"


Speech::Speech()
    : m_sp_voice( NULL )
{
    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
    ::CoCreateInstance( CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_sp_voice );

    po::options_description options;
    options.add_options()
        ( speech_name, po::wvalue<std::wstring>(), "speech name" )
        ;
    IConfigurationFile::instance().add_options_description( options ).add_observer( this );
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
        boost::lock_guard<boost::mutex> guard( m_mutex );
        m_sp_voice->Speak( word.c_str(), 0, NULL );
    }
}


void Speech::speak( const std::vector<std::wstring>& words )
{
    boost::lock_guard<boost::mutex> guard( m_mutex );

    for ( size_t i = 0; i < words.size(); ++i )
    {
        m_sp_voice->Speak( words[i].c_str(), 0, NULL );

        if ( i + 1 < words.size() )
        {
            boost::this_thread::sleep_for( boost::chrono::milliseconds(300) );
        }
    }
}


void Speech::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<std::wstring>( speech_name, vm, old ) )
    {
        boost::lock_guard<boost::mutex> guard( m_mutex );
        std::wstring attribs = L"Name=" + vm[speech_name].as<std::wstring>();;
        ISpObjectToken* token = NULL;

        if ( SUCCEEDED( SpFindBestToken( SPCAT_VOICES , attribs.c_str(), L"", &token ) ) )
        {
            ISpObjectToken* old_token = NULL;
            m_sp_voice->GetVoice( &old_token );
            m_sp_voice->SetVoice( token );
            old_token->Release();
        }
    }
}
