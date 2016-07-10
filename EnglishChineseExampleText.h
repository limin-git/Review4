#pragma once
#include "AbstructText.h"
#include "Singleton.h"
#include "IEnglishPlayer.h"


struct EnglishChineseExampleText : AbstructText
{
public:

    EnglishChineseExampleText( const fs::path& file_path );
    virtual void last_write_time_changed( const fs::path& file );

private:

    bool reload();
    size_t hash( const std::wstring& s );
    void notify();

private:

    bool m_hash_without_symbols;
    Singleton<IEnglishPlayer> m_player;
};
