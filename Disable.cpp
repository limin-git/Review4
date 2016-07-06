#include "stdafx.h"
#include "Disable.h"
#include "IConfigurationFile.h"

// singleton dependency:
// IDisable
//     --> IConfigurationFile

namespace po = boost::program_options;


Disable::Disable()
{
    po::options_description options;
    options.add_options()
        ( "file.disabled", po::wvalue<std::wstring>(), "disabled file" )
        ;
    m_file_name = IConfigurationFile::instance()
        .add_options_description( options )
        .variables_map()["file.disabled"].as<std::wstring>();
    fs::system_complete( m_file_name );
    load_file();
    //TODO: support multiple disable files
}


Disable::~Disable()
{
    m_file_stream.flush();
    m_file_stream.close();
}


void Disable::disable( ISlideshowPtr slideshow )
{
    // TODO: create 2 threads

    if ( is_disabled( slideshow->key() ) || 0 == slideshow->key() )
    {
        return;
    }

    m_file_stream << slideshow->key() << L"\t" << slideshow->key_string() << std::endl;
    m_disabled.insert( slideshow->key() );
    notify( slideshow->key() );
}


const std::set<size_t>& Disable::disabled()
{
    return m_disabled;
}


void Disable::notify( size_t key )
{
    BOOST_FOREACH( IDisableObserver* observer, m_observers )
    {
        observer->disabled( key );
    }
}


void Disable::load_file()
{
    std::wifstream is( m_file_name.wstring().c_str() );

    if ( is )
    {
        for ( std::wstring s; std::getline( is, s ); )
        {
            if ( !s.empty() )
            {
                size_t pos = s.find( '\t' );

                if ( pos != std::wstring::npos )
                {
                    try
                    {
                        size_t key = boost::lexical_cast<size_t>( s.substr( 0, pos ) );
                        m_disabled.insert( key );
                    }
                    catch ( std::exception& )
                    {
                    }
                }
            }
        }

        is.close();
    }

    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::app );
}


void Disable::add_observer( IDisableObserver* observer )
{
    m_observers.insert( observer );
}


void Disable::remove_observer( IDisableObserver* observer )
{
    m_observers.erase( observer );
}


bool Disable::is_disabled( size_t key )
{
    return ( m_disabled.find( key ) != m_disabled.end() );
}
