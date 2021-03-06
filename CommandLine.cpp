#include "stdafx.h"
#include "CommandLine.h"
namespace po = boost::program_options;

#define config_file     "config-file"
#define config          "config"
#define cfg             "cfg"
#define configuration   "configuration"


CommandLine::CommandLine()
    : m_options_description( "Usage" )
{
    m_options_description.add_options()
        ( config_file, po::wvalue< std::vector<std::wstring> >()->multitoken(),  "configuration files." )
        ( config, po::wvalue< std::vector<std::wstring> >()->multitoken(),  "configuration files." )
        ( cfg, po::wvalue< std::vector<std::wstring> >()->multitoken(),  "configuration files." )
        ( configuration, po::wvalue< std::vector<std::wstring> >()->multitoken(),  "configuration files." )
        ;
}


void CommandLine::parse_command_line( int argc, wchar_t* argv[] )
{
    po::positional_options_description options;
    options.add( config_file, -1 );

    try
    {
        store( po::wcommand_line_parser( argc, argv ).options( m_options_description ).positional( options ).allow_unregistered().run(), m_variables_map );
        notify( m_variables_map );
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
}
