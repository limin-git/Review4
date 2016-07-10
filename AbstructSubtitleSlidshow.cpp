#include "stdafx.h"
#include "AssSlideshow.h"


AbstructSubtitleSlidshow::AbstructSubtitleSlidshow( size_t key, const SubTime& start_t, const SubTime& end_t, const std::wstring& txt, Singleton<IConsole>& console, Singleton<IMoviePlayer>& player )
    : m_key( key ),
      start_time( start_t ),
      end_time( end_t ),
      text( txt ),
      m_console( console ),
      m_player( player )
{
}
