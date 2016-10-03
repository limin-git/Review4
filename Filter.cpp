#include "stdafx.h"
#include "Filter.h"
#include "IConfigurationFile.h"
#include "FileUtility.h"
namespace po = boost::program_options;

#define file_filter    "file.filter"


Filter::Filter()
{
    po::options_description options;
    options.add_options()
        ( file_filter, po::wvalue<std::wstring>(), "filter file" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).variables_map();

    if ( vm.count( file_filter ) )
    {
        m_file_name = vm[file_filter].as<std::wstring>();
        m_file_name = system_complete( m_file_name );
        load_file();
    }
}


Filter::~Filter()
{
}


void Filter::load_file()
{
    std::wstring s = Utility::wstring_from_file( m_file_name.wstring().c_str() );

    const boost::wregex e
    (
        L"(?x)"
        L" ^ [ \t¡¡]* ([^\\r\\n]+?) [ \t¡¡]* [\\r\\n]{1,2}"           // $1: english
    );

    boost::wsregex_iterator it( s.begin(), s.end(), e );
    boost::wsregex_iterator end;

    for ( ; it != end; ++it )
    {
        m_filter.insert( it->str(1) );
    }

    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::in | std::ios::out | std::ios::app );

    if ( ! m_file_stream )
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
    const std::wstring& str = slideshow->key_string();

    if ( ! is_filtered( str ) )
    {
        m_filter.insert( str );
        m_file_stream << str << std::endl;
    }
}
