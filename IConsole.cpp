#include "stdafx.h"
#include "Console.h"


static IConsole* s_console = NULL;


IConsole& IConsole::instance()
{
    if ( s_console == NULL )
    {
        s_console = new Console;
    }

    return *s_console;
}


void IConsole::remove()
{
    delete s_console;
    s_console = NULL;
}
