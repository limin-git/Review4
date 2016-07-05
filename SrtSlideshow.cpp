#include "stdafx.h"
#include "SrtSlideshow.h"
#include "IConsole.h"
#include "IMoviePlayer.h"


SrtSubtitle::SrtSubtitle( size_t key, size_t num, Time start_t, Time stop_t, const std::wstring& txt, const std::wstring& txt2 )
    : m_key( key ),
      number( num ),
      start_time( start_t ),
      stop_time( stop_t ),
      text( txt ),
      text2( txt2 )
{
}


bool SrtSubtitle::show()
{
    IConsole::instance()
        .cls()
        .write( text )
        .write( "\n" )
        .write( text2 );
    Time& t = start_time;
    IMoviePlayer::instance().go_to( t.hour, t.minute, t.second, t.millisecond, stop_time.total_millisecond - t.total_millisecond );
    return true;
}


void SrtSubtitle::clear_state()
{
}


size_t SrtSubtitle::key()
{
    return m_key;
}


const std::wstring& SrtSubtitle::key_string()
{
    return text;
}
