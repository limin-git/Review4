#pragma once
#include "AbstructText.h"
#include "IMoviePlayer.h"
#include "IConsole.h"
#include "Singleton.h"


struct AssSubtitleText : AbstructText
{
public:

    AssSubtitleText( const fs::path& file_path );

private:

    void parse();
    void trim_text( std::wstring& text );

private:

    Singleton<IConsole> m_console;
    Singleton<IMoviePlayer> m_player;
};
