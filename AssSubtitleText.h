#pragma once
#include "AbstructText.h"


struct AssSubtitleText : AbstructText
{
public:

    AssSubtitleText( const fs::path& file_path );

private:

    void parse();
    void trim_text( std::wstring& text );
};
