#include "stdafx.h"
#include "History.h"
#include "IConfigurationFile.h"
#include "FileUtility.h"
#include "IDisable.h"

// singleton dependency:
// IHistory
//     --> IConfigurationFile
//     --> IDisable


History::History()
    : m_no_history( false )
{
    po::options_description options;
    options.add_options()
        ( "file.history", po::wvalue<std::wstring>(), "history file" )
        ( "review.no-history", po::wvalue<std::wstring>(), "write history or not?" )
        ;
    po::variables_map& vm = m_configuration->add_options_description( options ).variables_map();

    if ( vm.count( "review.no-history" ) )
    {
        m_no_history = ( L"true" == vm["review.no-history"].as<std::wstring>() );
    }

    if ( ! m_no_history )
    {
        if ( vm.count( "file.history" ) )
        {
            m_file_name = vm["file.history"].as<std::wstring>();
        }

        m_file_name = system_complete( m_file_name );
        load_history();
        m_disable->add_observer( this );
    }
}


History::~History()
{
    if ( ! m_no_history )
    {
        save_history_file();
        m_disable->remove_observer( this );
    }
}


const std::vector<std::time_t>& History::history( size_t key )
{
    if ( m_no_history )
    {
        static std::vector<std::time_t> empty;
        return empty;
    }

    return m_history[key];
}


void History::write_history( size_t key, std::time_t t)
{
    if ( ! m_no_history )
    {
        m_history[key].push_back( t );
        m_file_stream << key << '\t' << t << std::endl;
    }
}


void History::save_history_file()
{
    m_file_stream.close();
    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::out | std::ios::trunc );

    if ( ! m_file_stream )
    {
        return;
    }

    BOOST_FOREACH( HistoryRecord::value_type& v, m_history )
    {
        if ( ! v.second.empty() )
        {
            m_file_stream << v.first << "\t";
            BOOST_FOREACH( std::time_t t, v.second ) { m_file_stream << t << "\t"; }
            m_file_stream << "\n";
        }
    }

    m_file_stream.close();
}


void History::load_history()
{
    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::in | std::ios::out | std::ios::app );

    if ( m_file_stream )
    {
        size_t key = 0;
        std::time_t t = 0;
        std::stringstream strm;

        for ( std::string s; std::getline( m_file_stream, s ); NULL )
        {
            if ( !s.empty() )
            {
                strm.clear();
                strm.str( s );
                strm >> key;

                if ( ! m_disable->is_disabled( key ) )
                {
                    std::vector<std::time_t>& times = m_history[key];

                    while( strm >> t )
                    {
                        times.push_back( t );
                    }
                }
            }
        }

        m_file_stream.clear();
    }
    else
    {
        m_file_stream.open( m_file_name.wstring().c_str(), std::ios::out | std::ios::app );
    }
}


void History::disabled( size_t key )
{
    m_history.erase( key );
}
