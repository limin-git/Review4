#include "stdafx.h"
#include "Filter.h"
#include "IConfigurationFile.h"
namespace po = boost::program_options;

#define file_filter    "file.filter"


Filter::Filter()
{
    po::options_description options;
    options.add_options()
        ( file_filter, po::wvalue<std::wstring>(), "filter file" )
        ;
    m_file_name = IConfigurationFile::instance()
        .add_options_description( options )
        .variables_map()[file_filter].as<std::wstring>();
    m_file_name = system_complete( m_file_name );
    load_file();
}


Filter::~Filter()
{
}


void Filter::load_file()
{
    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::in | std::ios::out | std::ios::app );

    if ( m_file_stream )
    {
        for ( std::wstring s; std::getline( m_file_stream, s ); )
        {
            if ( !s.empty() )
            {
                m_filter.insert( s );
            }
        }

        m_file_stream.clear();
    }
    else
    {
        m_file_stream.open( m_file_name.wstring().c_str(), std::ios::out | std::ios::app );
    }
}


bool Filter::is_filtered( const std::wstring& str )
{
    return ( m_filter.find( str ) != m_filter.end() );
}


void Filter::filter( ISlideshowPtr slideshow )
{
    // TODO
}
