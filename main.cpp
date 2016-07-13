#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "ILog.h"


int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc == 1 )
    {
        std::cout << ICommandLine::instance().options_description() << std::endl;
        system( "pause" );
        return 0;
    }

    ICommandLine::instance().parse_command_line( argc, argv );

    try
    {
        ILog::instance();
        IReviewManager::instance().run();
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        system( "pause" );
    }

	return 0;
}
