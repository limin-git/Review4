#include "stdafx.h"
#include "History.h"
#include "IConfigurationFile.h"
#include "FileUtility.h"


History::History()
{
    po::options_description desc;
    desc.add_options()
        ( "file.history", po::wvalue<std::wstring>(), "history file" )
        ;
    IConfigurationFile::instance().add_options_description( desc );
    m_file_name = IConfigurationFile::instance().variables_map()["file.history"].as<std::wstring>();
    load_history();
}


History::~History()
{
    save_history_file();
}


const std::vector<std::time_t>& History::history( size_t key )
{
    return m_history[key];
}


void History::write_history( size_t key, std::time_t t)
{
    m_history[key].push_back( t );
}


void History::save_history_file()
{
    std::ofstream os( m_file_name.wstring().c_str() );

    if ( !os )
    {
        return;
    }
    
    BOOST_FOREACH( HistoryRecord::value_type& v, m_history )
    {
        os << v.first << "\t";
        BOOST_FOREACH( std::time_t t, v.second ) { os << t << "\t"; }
        os << "\n";
    }
}


void History::load_history()
{
    std::ifstream is( m_file_name.wstring().c_str() );

    if ( !is )
    {
        return;
    }

    size_t key = 0;
    std::time_t t = 0;
    std::stringstream strm;

    for ( std::string s; std::getline( is, s ); NULL )
    {
        if ( !s.empty() )
        {
            strm.clear();
            strm.str( s );
            is >> key;
            std::vector<std::time_t>& times = m_history[key];

            while( strm >> t )
            {
                times.push_back( t );
            }
        }
    }
}
