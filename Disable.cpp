#include "stdafx.h"
#include "Disable.h"
#include "IConfigurationFile.h"
#include "IText.h"
namespace po = boost::program_options;

#define file_disable    "file.disabled"


Disable::Disable()
{
    po::options_description options;
    options.add_options()
        ( file_disable, po::wvalue<std::wstring>(), "disabled file" )
        ;
    m_file_name = IConfigurationFile::instance()
        .add_options_description( options )
        .variables_map()[file_disable].as<std::wstring>();
    m_file_name = system_complete( m_file_name );
    load_file();
    //TODO: support multiple disable files
}


Disable::~Disable()
{
}


void Disable::disable( ISlideshowPtr slideshow )
{
    // TODO: create 2 threads

    if ( is_disabled( slideshow->key_string() ) || 0 == slideshow->key() )
    {
        return;
    }

    m_file_stream << slideshow->key_string() << std::endl; // TODO: fix unicode fstream
    m_disabled.insert( slideshow->key_string() );
    notify( slideshow->key() );
}


const std::set<std::wstring>& Disable::disabled()
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
    m_file_stream.open( m_file_name.wstring().c_str(), std::ios::in | std::ios::out | std::ios::app );

    if ( m_file_stream )
    {
        for ( std::wstring s; std::getline( m_file_stream, s ); )
        {
            if ( !s.empty() )
            {
                m_disabled.insert( s );
            }
        }

        m_file_stream.clear();
    }
    else
    {
        m_file_stream.open( m_file_name.wstring().c_str(), std::ios::out | std::ios::app );
    }
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
    return is_disabled( IText::instance().slideshow( key )->key_string() );
}


bool Disable::is_disabled( const std::wstring& key )
{
    return ( m_disabled.find( key ) != m_disabled.end() );
}
