#pragma once
#include "AbstructText.h"


struct AssSubtitleText : AbstructText
{
public:

    AssSubtitleText( const fs::path& file_path );
    virtual bool parse_text();

private:

    void trim_text( std::wstring& text );
};
