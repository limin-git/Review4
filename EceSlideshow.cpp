#include "stdafx.h"
#include "EceSlideshow.h"
#include "IConsole.h"
#include "IEnglishPlayer.h"


EceSlideshow::EceSlideshow( size_t key, const std::wstring& eng, const std::wstring& chs, const std::wstring& exp )
    : m_key( key ),
      m_english( eng ),
      m_chinese( chs ),
      m_example( exp ),
      m_stage( English )
{
}


bool EceSlideshow::show()
{
    switch ( m_stage )
    {
    case English:
        IConsole::instance()
            .cls()
            .write_center( m_english )
            ;
        IEnglishPlayer::instance().speak( m_english );
        m_stage = Chinese;
        return false;

    case Chinese:
        IConsole::instance()
            .cls()
            .write( m_english ).write( "\n" )
            .write( m_chinese ).write( "\n" )
            ;
        m_stage = Example;
        return false;

    case Example:
    default:
        IConsole::instance()
            .write( m_example )
            ;
        m_stage = English;
        return true;
    }
}


size_t EceSlideshow::key()
{
    return m_key;
}


const std::wstring& EceSlideshow::key_string()
{
    return m_english;
}
