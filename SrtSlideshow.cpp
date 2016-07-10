#include "stdafx.h"
#include "SrtSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


SrtSlideshow::SrtSlideshow( size_t key, size_t num, SubTime start_t, SubTime end_t, const std::wstring& txt, const std::wstring& txt2, Singleton<IConsole>& console, Singleton<IMoviePlayer>& player )
    : AbstructSubtitleSlidshow( key, start_t, end_t, txt, console, player )
{
}


bool SrtSlideshow::show()
{
    m_console->cls().write( "\n" ).write( "\t" ).write( text ).write( "\n" ).write( "\t" ).write( text2 );
    m_player->play( ISubtitleSlideshowPtr( new SrtSlideshow(*this) ) );
    return true;
}
