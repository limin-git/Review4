#pragma once
#include "AbstructText.h"


struct SrtSubtitleText : AbstructText
{
    SrtSubtitleText( const fs::path& file_path );
    virtual bool parse_text();
};
