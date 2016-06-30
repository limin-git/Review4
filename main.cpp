#include "stdafx.h"
#include "ICommandLine.h"


int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc == 1 )
    {
        std::cout << ICommandLine::instance().get_options_description() << std::endl;
        return 0;
    }

    ICommandLine::instance().parse_command_line( argc, argv );

    namespace po = boost::program_options;
    po::variables_map& vm = ICommandLine::instance().get_variables_map();

    if ( vm.count( "config-file" ) )
    {
        std::vector<std::wstring> vs = vm[ "config-file" ].as< std::vector<std::wstring> >();

        for ( size_t i = 0; i < vs.size(); ++i )
        {
            std::wcout << vs[i] << ", ";
        }

        std::cout << std::endl;
    }

	return 0;
}
