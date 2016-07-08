#include "stdafx.h"
#include "SrtSubtitleText.h"
#include "FileUtility.h"
#include "SrtSlideshow.h"
#include "IDisable.h"


SrtSubtitleText::SrtSubtitleText( const fs::path& file_path )
    : AbstructText( file_path )
{
    parse();
}


void SrtSubtitleText::parse()
{
    std::wstring s = Utility::wstring_from_file( m_file_path.wstring().c_str() );
    boost::wsmatch m;
    boost::wregex e
    (
        L"(?x)"
        L"^ ([0-9]+) [\\r\\n]+"         // $1: number
        L"^ ([^\\r\\n]+) [\\r\\n]+"     // $2 start-stop time
        L"^ ([^\\r\\n]+) [\\r\\n]+"     // $3 text
        L"^ ([^\\r\\n]+)*"              // $4 text2
    );

    boost::wregex time_e
    (
        L"([0-9]+):([0-9]+):([0-9]+),([0-9]+) --> ([0-9]+):([0-9]+):([0-9]+),([0-9]+)"
    );

    boost::wsregex_iterator it( s.begin(), s.end(), e );
    boost::wsregex_iterator end;

    for ( ; it != end; ++it )
    {
        std::wstring text = it->str(3);
        size_t key = m_hash( text );

        if ( IDisable::instance().is_disabled( key ) )
        {
            continue;
        }

        size_t number = boost::lexical_cast<size_t>( it->str(1) );
        SubTime start_time;
        SubTime end_time;
        std::wstring time_string = it->str(2);
        boost::wsmatch m;

        if ( boost::regex_search( time_string, m, time_e ) )
        {
            start_time.hour           = boost::lexical_cast<size_t>( m.str(1) );
            start_time.minute         = boost::lexical_cast<size_t>( m.str(2) );
            start_time.second         = boost::lexical_cast<size_t>( m.str(3) );
            start_time.millisecond    = boost::lexical_cast<size_t>( m.str(4) );
            start_time.milliseconds = ( start_time.hour*3600 + start_time.minute*60 + start_time.second ) * 1000 + start_time.millisecond;

            end_time.hour             = boost::lexical_cast<size_t>( m.str(5) );
            end_time.minute           = boost::lexical_cast<size_t>( m.str(6) );
            end_time.second           = boost::lexical_cast<size_t>( m.str(7) );
            end_time.millisecond      = boost::lexical_cast<size_t>( m.str(8) );
            end_time.milliseconds = ( end_time.hour*3600 + end_time.minute*60 + end_time.second ) * 1000 + end_time.millisecond;
        }

        std::wstring text2;

        if ( (*it)[4].matched )
        {
            text2 = it->str(4);
        }

        m_keys.push_back( key );
        m_slidshow_map[key] = ISlideshowPtr( new SrtSlideshow( key, number, start_time, end_time, text, text2 ) );
    }
}
