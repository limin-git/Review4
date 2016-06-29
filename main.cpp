#include "stdafx.h"
#include "CommandLine.h"


int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc == 1 )
    {
        std::cout << g_command_line.m_desc << std::endl;
        return 0;
    }

    g_command_line.parse_command_line( argc, argv );

    namespace po = boost::program_options;
    po::variables_map& vm = g_command_line.m_vm;

    if ( vm.count( "config-file" ) )
    {
        std::vector<std::wstring> vs = vm[ "config-file" ].as< std::vector<std::wstring> >();
        //std::copy( vs.begin(), vs.end(), std::ostream_iterator<std::wstring>( std::wcout, ", " ) );
        for ( size_t i = 0; i < vs.size(); ++i )
        {
            std::wcout << vs[i] << ", ";
        }
        std::cout << std::endl;
    }

	return 0;
}
