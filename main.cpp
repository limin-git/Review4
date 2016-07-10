#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "IInput.h"
#include "IConsole.h"
#include "IScheduler.h"
#include "IHistory.h"
#include "ILog.h"
#include "IConfigurationFile.h"
#include "IText.h"
#include "IDisable.h"
#include "IWallpaper.h"
#include "ITextFactory.h"
#include "IMoviePlayer.h"
#include "IHotKey.h"


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
        IWallpaper::instance().run();
        IInput::instance().run();
    }
    catch ( std::exception& e)
    {
        IConsole::remove();
        std::cout << "error: " << e.what() << std::endl;
        std::cout << IConfigurationFile::instance().options_description() << std::endl;
        system( "pause" );
    }

    try
    {
        IReviewManager::remove();
        IWallpaper::remove();
        IScheduler::remove();
        IHistory::remove();
        IText::remove(); ITextFactory::remove();
        IDisable::remove();
        IMoviePlayer::remove();
        IInput::remove(); IHotKey::remove();
        IConfigurationFile::remove();
        ICommandLine::remove();
        IConsole::remove();
        ILog::remove();
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
        system( "pause" );
    }

	return 0;
}
