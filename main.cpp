#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "ILog.h"


int wmain(int argc, wchar_t* argv[])
{
    if ( argc == 1 )
    {
        std::cout << ICommandLine::instance().options_description() << std::endl;
        system( "pause" );
        return 0;
    }

    ICommandLine::instance().parse_command_line( argc, argv );
    ILog::instance();

    try
    {
        IReviewManager::instance().run();
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        system( "pause" );
    }

	return 0;
}
