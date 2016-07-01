#include "stdafx.h"
#include "TestConsole.h"
#include "IConsole.h"


TestConsole::TestConsole()
{
    test1();
}


void TestConsole::test1()
{
    IConsole::instance();
    IConsole::instance().write_center( "hello, world" );
    _getch();
    IConsole::instance().cls();
    IConsole::instance().write( "hello, world" );
    _getch();
    IConsole::instance().write_center( L"中华人民共和国" );
    _getch();
    IConsole::instance().cls();
    IConsole::instance().write( L"中华人民共和国" );
    _getch();
    IConsole::remove();
}
