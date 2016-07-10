#include "stdafx.h"
#include "AssSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


AssSlideshow::AssSlideshow( size_t key, const SubTime& start_t, const SubTime& end_t, const std::wstring& txt, Singleton<IConsole>& console, Singleton<IMoviePlayer>& player )
    : AbstructSubtitleSlidshow( key, start_t, end_t, txt, console, player )
{
}


bool AssSlideshow::show()
{
    m_console->cls().write( "\n" ).write( "\t" ).write( text );
    m_player->play( ISubtitleSlideshowPtr( new AssSlideshow(*this) ) );
    return true;
}
