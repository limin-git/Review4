#pragma once
#include "AbstructText.h"


struct SrtSubtitleText : AbstructText
{
public:

    SrtSubtitleText( const fs::path& file_path );

private:

    void parse();
};
