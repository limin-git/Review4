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
        .write( "\n" ).write( text )
        ;
    StartEndTime& s = start_time;
    StartEndTime& e = end_time;
    size_t duration = ( s.milliseconds < e.milliseconds ? e.milliseconds - s.milliseconds : 1000 );
    IMoviePlayer::instance().go_to( s.hour, s.minute, s.second, s.millisecond, duration );
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
