#include "stdafx.h"
#include "EceSlideshow.h"
#include "IEnglishPlayer.h"
#include "IConsole.h"


EceSlideshow::EceSlideshow( size_t key, const std::wstring& eng, const std::wstring& chs, const std::wstring& exp, size_t lines, size_t indents )
    : m_key( key ),
      m_english( eng ),
      m_chinese( chs ),
      m_example( exp ),
      m_stage( English )
{
    std::wstringstream strm;
    strm
        << L"\n"
        << L"\t" << m_english << std::wstring( lines, L'\n' )
        << std::wstring( indents, L'\t' ) << m_chinese << L"\n"
        << std::wstring( indents, L'\t' ) << boost::replace_all_copy( m_example, L"\n", L"\n" + std::wstring( indents, L'\t' ) );
    m_cache = strm.str();
}


bool EceSlideshow::show()
{
    switch ( m_stage )
    {
    case English:
        {
            //std::wstringstream strm( m_english );
            //for ( size_t i = 0; i < 20; ++i )
            //{
            //    strm << "    " << m_english;
            //}
            //IConsole::instance().cls().write_center( strm.str() );
            IConsole::instance().cls().write_center( m_english );
            IEnglishPlayer::instance().speak( m_english );
            m_stage = Chinese;
        }
        return false;

    case Chinese:
        IConsole::instance().cls().write( m_cache );
        m_stage = English;
        return true;

    case Example:
    default:
        IConsole::instance().write( m_example );
        m_stage = English;
        return true;
    }
}


bool EceSlideshow::show_all()
{
    IConsole::instance().cls().write( m_cache );
    IEnglishPlayer::instance().speak( m_english );
    m_stage = English;
    return true;
}


void EceSlideshow::clear_state()
{
    m_stage = English;
}


size_t EceSlideshow::key()
{
    return m_key;
}


const std::wstring& EceSlideshow::key_string()
{
    return m_english;
}
