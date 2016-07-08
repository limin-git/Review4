#include "stdafx.h"
#include "AssSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


AssSlideshow::AssSlideshow( size_t key, const SubTime& start_t, const SubTime& end_t, const std::wstring& txt )
    : AbstructSubtitleSlidshow( key, start_t, end_t, txt )
{
}


bool AssSlideshow::show()
{
    IConsole::instance()
        .cls().write( "\n" )
        .write( "\t" ).write( text )
        ;
    IMoviePlayer::instance().play( ISubtitleSlideshowPtr( new AssSlideshow(*this) ) );
    return true;
}
