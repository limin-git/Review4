#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "IInput.h"
#include "IConsole.h"
#include "IScheduler.h"
#include "IHistory.h"
#include "IEnglishPlayer.h"
#include "ISpeech.h"
#include "ISound.h"
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
        std::cout << ICommandLine::instance().get_options_description() << std::endl;
        return 0;
    }

    ICommandLine::instance().parse_command_line( argc, argv );

    try
    {
        IReviewManager::instance().run();
        IWallpaper::instance().run();
        IInput::instance().run();
    }
    catch ( std::exception& e)
    {
        IConsole::remove();
        std::cout << "error: " << e.what() << std::endl;
        std::cout << IConfigurationFile::instance().options_description() << std::endl;
    }

    {
        IReviewManager::remove();
        IWallpaper::remove();
        IScheduler::remove();
        IHistory::remove();
        IText::remove(); ITextFactory::remove();
        IDisable::remove();
        IEnglishPlayer::remove(); ISpeech::remove(); ISound::remove();
        IMoviePlayer::remove();
        IInput::remove(); IHotKey::remove();
        IConfigurationFile::remove();
        ICommandLine::remove();
        IConsole::remove();
        ILog::remove();
    }

	return 0;
}
