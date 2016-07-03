#include "stdafx.h"
#include "EnglishChineseExampleText.h"
#include "IFileChangeManager.h"
#include "FileUtility.h"
#include "AlgorithmUtility.h"
#include "EceSlideshow.h"


EnglishChineseExampleText::EnglishChineseExampleText( const boost::filesystem::path& file_path )
    : m_file_path( file_path )
{
    reload();
    IFileChangeManager::instance().add_handler( file_path, this );
}


ISlideshowPtr EnglishChineseExampleText::slideshow( size_t key )
{
    return m_slidshow_map[key];
}


const std::set<size_t>& EnglishChineseExampleText::keys()
{
    return m_keys;
}


void EnglishChineseExampleText::reload()
{
    std::wstring s = Utility::wstring_from_file( m_file_path.wstring().c_str() );

    if ( s == m_string )
    {
        return;
    }

    std::set<size_t> keys;
    std::map<size_t, ISlideshowPtr> slidshow_map;

    static const boost::wregex e
    (
        L"(?x)"
        L"^ [ \t¡¡]* ([^ \t¡¡\\r\\n] [^\\r\\n]*?) [ \t¡¡]* [\\r\\n]{1,2}"           // $1: english
        L"^ [ \t¡¡]* ([^ \t¡¡\\r\\n] [^\\r\\n]*?) [ \t¡¡]* [\\r\\n]{1,2}"           // $2: chinese
        L"((?:^[ \t¡¡]*  [^ \t¡¡\\r\\n] [^\\r\\n]*?  [ \t¡¡]* [\\r\\n]{1,2})*)"     // $3: example
    );

    boost::wsregex_iterator it( s.begin(), s.end(), e );
    boost::wsregex_iterator end;

    for ( ; it != end; ++it )
    {
        const std::wstring& english = it->str(1);
        const std::wstring& chinese = it->str(2);
        const std::wstring& example = ( (*it)[3].matched ? it->str(3) : L"" );
        ISlideshowPtr slideshow( new EceSlideshow( english, chinese, example ) );
        size_t key = hash( english );
        keys.insert( key );
        slidshow_map[key] = slideshow;
    }

    m_string.swap( s );
    m_keys.swap( keys );
    m_slidshow_map.swap( slidshow_map );
}


size_t EnglishChineseExampleText::hash( std::wstring s )
{
    static const wchar_t* symbols =
        L" \"\',.?:;!-/#()|<>{}[]~`@$%^&*+\n\t\r"
        L"¡¡£¬¡£¡¢£¿£¡£»£º¡¤£®¡°¡±¡®¡¯£à£­£½¡«£À£££¤£¥£ª£ß£«£ü¡ª¡ª¡ª¡­¡­¡­¡¶¡·£¨£¨¡¾¡¿¡¸¡¹¡º¡»¡¼¡½¡´¡µ£û£ý";
    Utility::remove_if_is_any_of( s, symbols );
    return m_hash( s );
}


void EnglishChineseExampleText::last_write_time_changed( const boost::filesystem::path& file )
{
    if ( file == m_file_path )
    {
        reload();
    }
}
