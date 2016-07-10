#include "stdafx.h"
#include "EnglishChineseExampleText.h"
#include "IFileChangeManager.h"
#include "FileUtility.h"
#include "AlgorithmUtility.h"
#include "EceSlideshow.h"
#include "EmptySlideshow.h"
#include "IDisable.h"
#include "IConfigurationFile.h"

namespace po = boost::program_options;

// singleton dependency:
// IText
//     --> IFileChangeManager
//     --> IDisable
//     --> IConsole


EnglishChineseExampleText::EnglishChineseExampleText( const fs::path& file_path )
    : AbstructText( file_path ),
      m_hash_without_symbols( true )
{
    m_file_path = system_complete( m_file_path );

    po::options_description options( "Advanced" );
    options.add_options()
        ( "advanced.hash-without-symbols", po::wvalue<std::wstring>(), "trim symbols when hash the key string" )
        ;
    po::variables_map& vm = m_configuration->add_options_description( options ).variables_map();

    if ( vm.count( "advanced.hash-without-symbols" ) )
    {
        m_hash_without_symbols = L"true" == vm[ "advanced.hash-without-symbols" ].as<std::wstring>();
    }

    reload();
}


bool EnglishChineseExampleText::reload()
{
    std::wstring s = Utility::wstring_from_file( m_file_path.wstring().c_str() );

    if ( s == m_string )
    {
        return false;
    }

    KeyList keys;
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

        if ( ! m_disable->is_disabled( key ) )
        {
            keys.push_back( key );
            slidshow_map[key] = ISlideshowPtr( new EceSlideshow( key, english, chinese, example, m_console, m_player ) );
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


size_t EnglishChineseExampleText::hash( const std::wstring& s )
{
    if ( m_hash_without_symbols )
    {
        std::wstring ts = s;
        static const wchar_t* symbols =
            L" \"\',.?:;!-/#()|<>{}[]~`@$%^&*+\n\t\r"
            L"¡¡£¬¡£¡¢£¿£¡£»£º¡¤£®¡°¡±¡®¡¯£à£­£½¡«£À£££¤£¥£ª£ß£«£ü¡ª¡ª¡ª¡­¡­¡­¡¶¡·£¨£¨¡¾¡¿¡¸¡¹¡º¡»¡¼¡½¡´¡µ£û£ý";
        Utility::remove_if_is_any_of( ts, symbols );
        return m_hash( ts );
    }

    return m_hash( s );
}


void EnglishChineseExampleText::last_write_time_changed( const fs::path& file )
{
    if ( reload() )
    {
        notify();
    }
}


void EnglishChineseExampleText::notify()
{
    BOOST_FOREACH( ITextObserver* observer, m_observers )
    {
        observer->text_changed( this );
    }
}
