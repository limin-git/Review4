#pragma once
#include "IEnglishPlayer.h"
#include "QueueProcessor.h"
#include "IConfigurationFileObserver.h"
#include "ISpeech.h"
#include "ISound.h"
#include "IConfigurationFile.h"
#include "Singleton.h"

namespace fs = boost::filesystem;


struct EnglishPlayer : IEnglishPlayer,
                       IConfigurationFileObserver
{
public:

    EnglishPlayer();
    ~EnglishPlayer();
    virtual void speak( const std::wstring& word );
    virtual void speak( const std::vector<std::wstring>& words );
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:

    struct Word
    {
        std::wstring word;
        boost::filesystem::path path;
    };

    typedef std::vector<Word> Words;

    void speak_impl( const Word& word );

private:

    typedef std::map<fs::path, std::wstring> SpeechDirectory;
    SpeechDirectory m_speech_directories; // path, extension
    QueueProcessor m_processor;
    po::options_description m_options_description;
    Singleton<ISpeech> m_speech;
    Singleton<ISound> m_sound;
    Singleton<IConfigurationFile> m_configuration;
    bool m_disabled;
};
