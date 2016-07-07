#include "stdafx.h"
#include "SrtSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


SrtSlideshow::SrtSlideshow( size_t key, size_t num, StartEndTime start_t, StartEndTime end_t, const std::wstring& txt, const std::wstring& txt2 )
    : m_key( key ),
      number( num ),
      start_time( start_t ),
      end_time( end_t ),
      text( txt ),
      text2( txt2 )
{
}


bool SrtSlideshow::show()
{
    IConsole::instance()
        .cls().write( "\n" )
        .write( "\t" ).write( text ).write( "\n" )
        .write( "\t" ).write( text2 );
    StartEndTime& s = start_time;
    StartEndTime& e = end_time;
    size_t duration = ( s.milliseconds < e.milliseconds ? e.milliseconds - s.milliseconds : 1000 );
    IMoviePlayer::instance().go_to( s.hour, s.minute, s.second, s.millisecond, duration );
    return true;
}


void SrtSlideshow::clear_state()
{
}


size_t SrtSlideshow::key()
{
    return m_key;
}


const std::wstring& SrtSlideshow::key_string()
{
    return text;
}
