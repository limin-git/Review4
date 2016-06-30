#include "stdafx.h"
#include "ICommandLine.h"
#include "CommandLine.h"


static ICommandLine* g_command_line = NULL;


ICommandLine& ICommandLine::instance()
{
    if ( g_command_line == NULL )
    {
        g_command_line = new CommandLine;
    }

    return *g_command_line;
}


void ICommandLine::remove()
{
    if ( g_command_line != NULL )
    {
        delete g_command_line;
        g_command_line = NULL;
    }
}
