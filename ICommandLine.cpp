#include "stdafx.h"
#include "ICommandLine.h"
#include "CommandLine.h"


ICommandLine& ICommandLine::instance()
{
    static CommandLine command_line;
    return command_line;
}
