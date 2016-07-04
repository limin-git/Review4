#include "stdafx.h"
#include "Disable.h"
#include "IConfigurationFile.h"

namespace po = boost::program_options;


Disable::Disable()
{
    po::options_description desc;
    desc.add_options()
        ( "file.disabled", po::wvalue<std::wstring>(), "disabled file" )
        ;
    m_file_name = IConfigurationFile::instance()
        .add_options_description( desc )
        .variables_map()["file.disabled"].as<std::wstring>();
    fs::system_complete( m_file_name );
    //TODO: support multiple disable files
}


Disable::~Disable()
{

}


void Disable::disable( ISlideshowPtr slideshow )
{
    std::wofstream os( m_file_name.wstring().c_str(), std::ios::app );

    if ( os )
    {
        os << slideshow->key() << "\n" << slideshow->key_string() << std::endl;
    }

    notify( slideshow->key() );
}


const std::set<size_t>& Disable::disabled()
{
    return m_disabled;
}


void Disable::notify( size_t key )
{
}


void Disable::load_file()
{
    std::wifstream is( m_file_name.wstring().c_str() );

    if ( !is )
    {
        return;
    }


}
