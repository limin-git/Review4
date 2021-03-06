#include "stdafx.h"
#include "AssSubtitleText.h"
#include "FileUtility.h"
#include "AssSlideshow.h"
#include "IDisable.h"


AssSubtitleText::AssSubtitleText( const fs::path& file_path )
    : AbstructText( file_path )
{
}


bool AssSubtitleText::parse_text()
{
    std::wstring s = Utility::wstring_from_file( m_file_path.wstring().c_str() );
    boost::wsmatch m;
    boost::wregex e
    (
        L"(?x)"
        // Format:      Layer,  Start,   End,     Style,  Name,  MarginL, MarginR, MarginV, Effect, Text
        L"Dialogue:[ ]* [0-9]+, ([^,]+), ([^,]+), [^,]+,  [^,]*, [^,]*,   [^,]*,   [^,]*,   [^,]*,  ([^\\n\\r]+) [\\n\\r]+"
    );

    boost::wregex time_e
    (
        L"([0-9]+):([0-9]+):([0-9]+).([0-9]+)"
    );

    typedef std::map<SubTime, size_t> TimeKeyMap;
    TimeKeyMap time_key_map;

    boost::wsregex_iterator it( s.begin(), s.end(), e );
    boost::wsregex_iterator end;

    for ( ; it != end; ++it )
    {
        std::wstring text = it->str(3);

        if ( text.find( L"{\\p0}" ) != std::wstring::npos )
        {
            continue;
        }

        size_t key = m_hash( it->str() );

        if ( IDisable::instance().is_disabled( it->str() ) )
        {
            continue;
        }

        SubTime start_time;
        SubTime end_time;
        std::wstring start_time_str = it->str(1);
        std::wstring end_time_str = it->str(2);
        boost::wsmatch m;

        if ( boost::regex_search( start_time_str, m, time_e ) )
        {
            start_time.hour           = boost::lexical_cast<size_t>( m.str(1) );
            start_time.minute         = boost::lexical_cast<size_t>( m.str(2) );
            start_time.second         = boost::lexical_cast<size_t>( m.str(3) );
            start_time.millisecond    = boost::lexical_cast<size_t>( m.str(4) ) * 10;
            start_time.milliseconds = ( start_time.hour*3600 + start_time.minute*60 + start_time.second ) * 1000 + start_time.millisecond;
        }

        if ( boost::regex_search( end_time_str, m, time_e ) )
        {

            end_time.hour             = boost::lexical_cast<size_t>(m.str(1) );
            end_time.minute           = boost::lexical_cast<size_t>(m.str(2) );
            end_time.second           = boost::lexical_cast<size_t>(m.str(3) );
            end_time.millisecond      = boost::lexical_cast<size_t>(m.str(4) ) * 10;
            end_time.milliseconds = ( end_time.hour*3600 + end_time.minute*60 + end_time.second ) * 1000 + end_time.millisecond;
        }

        if ( end_time.milliseconds - start_time.milliseconds < 500 )
        {
            continue;
        }

        trim_text( text );
        time_key_map[start_time] = key;
        m_slidshow_map[key] = ISlideshowPtr( new AssSlideshow( key, start_time, end_time, text ) );
    }

    BOOST_FOREACH( TimeKeyMap::value_type& v, time_key_map )
    {
        m_keys.push_back( v.second );
    }

    return true;
}


void AssSubtitleText::trim_text( std::wstring& text )
{
    static const boost::wregex e
    (
        L"\\{[^\\{]+\\}"
    );

    text = boost::regex_replace( text, e, L"" );

    static const boost::wregex e2
    (
        L"[ \t]*\\\\[nN][ \t]*"
    );

    text = boost::regex_replace( text, e2, L"\n\t" );
}
