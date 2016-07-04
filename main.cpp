#include "stdafx.h"
#include "ICommandLine.h"
#include "IReviewManager.h"
#include "IInput.h"
#include "IConsole.h"


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

	return 0;
}
