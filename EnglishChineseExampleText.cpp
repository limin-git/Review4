#include "stdafx.h"
#include "EnglishChineseExampleText.h"
#include "IFileChangeManager.h"
#include "FileUtility.h"
#include "AlgorithmUtility.h"
#include "EceSlideshow.h"
#include "EmptySlideshow.h"
#include "IDisable.h"

// singleton dependency:
// IText
//     --> IFileChangeManager
//     --> IDisable
//     --> IConsole


EnglishChineseExampleText::EnglishChineseExampleText( const boost::filesystem::path& file_path )
    : m_file_path( file_path )
{
    reload();
    IFileChangeManager::instance().add_handler( file_path, this );
    IDisable::instance().add_observer( this );
}


EnglishChineseExampleText::~EnglishChineseExampleText()
{
    IDisable::instance().remove_observer( this );
}


ISlideshowPtr EnglishChineseExampleText::slideshow( size_t key )
{
    if ( m_keys.find( key ) == m_keys.end() )
    {
        return ISlideshowPtr( new EmptySlideshow );
    }

    return m_slidshow_map[key];
}


const std::set<size_t>& EnglishChineseExampleText::keys()
{
    return m_keys;
}


bool EnglishChineseExampleText::reload()
{
    std::wstring s = Utility::wstring_from_file( m_file_path.wstring().c_str() );

    if ( s == m_string )
    {
        return false;
    }

    std::set<size_t> keys;
    std::map<size_t, ISlideshowPtr> slidshow_map;

    static const boost::wregex e
    (
        L"(?x)"
        L"  ^[ \t¡¡]* ([^ \t¡¡\\r\\n] [^\\r\\n]*?) [ \t¡¡]* [\\r\\n]{1,2}"           // $1: english
        L"  ^[ \t¡¡]* ([^ \t¡¡\\r\\n] [^\\r\\n]*?) [ \t¡¡]* [\\r\\n]{1,2}"           // $2: chinese
        L"((^[ \t¡¡]*  [^ \t¡¡\\r\\n] [^\\r\\n]*?  [ \t¡¡]* [\\r\\n]{1,2})*)"        // $3: example
    );

    boost::wsregex_iterator it( s.begin(), s.end(), e );
    boost::wsregex_iterator end;

    for ( ; it != end; ++it )
    {
        const std::wstring& english = it->str(1);
        const std::wstring& chinese = it->str(2);
        const std::wstring& example = ( (*it)[3].matched ? it->str(3) : L"" );
        size_t key = hash( english );

        if ( !IDisable::instance().is_disabled( key ) )
        {
            keys.insert( key );
            slidshow_map[key] = ISlideshowPtr( new EceSlideshow( key, english, chinese, example ) );
        }
    }

    m_string.swap( s );
    m_slidshow_map.swap( slidshow_map );

    if ( m_keys == keys )
    {
        return false;
    }

    m_keys.swap( keys );
    return true;
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
        if ( reload() )
        {
            notify();
        }
    }
}


void EnglishChineseExampleText::disabled( size_t key )
{
    m_keys.erase( key );
    m_slidshow_map.erase( key );
}


void EnglishChineseExampleText::add_observer( ITextObserver* observer )
{
    m_observers.insert( observer );
}


void EnglishChineseExampleText::remove_observer( ITextObserver* observer )
{
    m_observers.erase( observer );
}


void EnglishChineseExampleText::notify()
{
    BOOST_FOREACH( ITextObserver* observer, m_observers )
    {
        observer->text_changed( this );
    }
}
