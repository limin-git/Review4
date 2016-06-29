#include "stdafx.h"
#include "CommandLine.h"
namespace po = boost::program_options;


CommandLine g_command_line;


CommandLine::CommandLine()
    : m_desc( "Usage" )
{
    m_desc.add_options()
        ( "config-file,c", po::wvalue< std::vector<std::wstring> >()->multitoken(),  "configuration files." )
        ;
}


void CommandLine::parse_command_line( int argc, wchar_t* argv[] )
{
    po::positional_options_description pdesc;
    pdesc.add( "config-file", -1 );

    try
    {
        store( po::wcommand_line_parser( argc, argv ).options( m_desc ).positional( pdesc ).allow_unregistered().run(), m_vm );
        notify( m_vm );
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
}
