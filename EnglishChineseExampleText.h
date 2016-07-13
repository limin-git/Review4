#pragma once
#include "AbstructText.h"


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
};
