#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "IInput.h"
#include "ILog.h"
#include "IWallpaper.h"
#include "Singleton.h"


int _tmain(int argc, _TCHAR* argv[])
{
    Singleton<ICommandLine> command_line;

    if ( argc == 1 )
    {
        std::cout << command_line->options_description() << std::endl;
        system( "pause" );
        return 0;
    }

    command_line->parse_command_line( argc, argv );

    try
    {
        Singleton<ILog> log;
        Singleton<IInput> input;
        Singleton<IWallpaper> wallpaper;
        Singleton<IReviewManager> review_manager;

        review_manager->run();
        wallpaper->run();
        input->run();
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        system( "pause" );
    }

	return 0;
}
