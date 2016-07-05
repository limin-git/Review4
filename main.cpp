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


int _tmain(int argc, _TCHAR* argv[])
{
    if ( argc == 1 )
    {
        std::cout << ICommandLine::instance().get_options_description() << std::endl;
        return 0;
    }

    ICommandLine::instance().parse_command_line( argc, argv );
    IConsole::instance();
    IReviewManager::instance().run();
    IInput::instance().run();

    {
        IReviewManager::remove();
        IScheduler::remove();
        IHistory::remove();
        IText::remove();
        IDisable::remove();
        IEnglishPlayer::remove();
        ISpeech::remove(); ISound::remove();
        IInput::remove();
        IConsole::remove();
        ILog::remove();
        IConfigurationFile::remove();
        ICommandLine::remove();
    }

	return 0;
}
