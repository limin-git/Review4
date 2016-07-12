#pragma once
#include "AbstructText.h"
#include "Singleton.h"
#include "IEnglishPlayer.h"
#include "IConfigurationFile.h"
#include "IConsole.h"


struct EnglishChineseExampleText : AbstructText
{
public:

    EnglishChineseExampleText( const fs::path& file_path );
    virtual void last_write_time_changed( const fs::path& file );
    virtual bool parse_text();

private:

    size_t hash( const std::wstring& s );
    void notify();

private:

    bool m_hash_without_symbols;
    Singleton<IConsole> m_console;
    Singleton<IEnglishPlayer> m_player;
    Singleton<IConfigurationFile> m_configuration;
};
