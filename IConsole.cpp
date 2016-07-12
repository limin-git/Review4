#include "stdafx.h"
#include "Console.h"


IConsole& IConsole::instance()
{
    static Console console;
    return console;
}
