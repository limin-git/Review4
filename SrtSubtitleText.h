#pragma once
#include "AbstructText.h"
#include "IMoviePlayer.h"
#include "Singleton.h"
#include "IConsole.h"


struct SrtSubtitleText : AbstructText
{
public:

    SrtSubtitleText( const fs::path& file_path );
    virtual bool parse_text();

private:

    Singleton<IConsole> m_console;
    Singleton<IMoviePlayer> m_player;
};
