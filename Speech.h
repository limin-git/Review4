#pragma once
#include "ISpeech.h"
#include "IConfigurationFileObserver.h"
#include <sapi.h>


struct Speech : ISpeech,
                IConfigurationFileObserver
{
public:

    Speech();
    ~Speech();
    virtual void speak( const std::wstring& word );
    virtual void speak( const std::vector<std::wstring>& words );
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:

    ISpVoice* m_sp_voice;
    std::wstring m_name;
    boost::mutex m_mutex;
};
