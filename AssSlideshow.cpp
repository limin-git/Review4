#include "stdafx.h"
#include "AssSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


AssSlideshow::AssSlideshow( size_t key, const StartEndTime& start_t, const StartEndTime& end_t, const std::wstring& txt )
    : m_key( key ),
      start_time( start_t ),
      end_time( end_t ),
      text( txt )
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


void AssSlideshow::clear_state()
{
}


size_t AssSlideshow::key()
{
    return m_key;
}


const std::wstring& AssSlideshow::key_string()
{
    return text;
}
