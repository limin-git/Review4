#include "stdafx.h"
#include "Console.h"


static IConsole* g_console = NULL;


IConsole& IConsole::instance()
{
    if ( g_console == NULL )
    {
        g_console = new Console;
    }

    return *g_console;
}


void IConsole::remove()
{
    if ( g_console != NULL )
    {
        delete g_console;
        g_console = NULL;
    }
}
