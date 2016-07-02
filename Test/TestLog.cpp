#include "stdafx.h"
#include "TestLog.h"
#include "ILog.h"
#include "ICommandLine.h"


TestLog::TestLog()
{
    test1();
}


void TestLog::test1()
{
    std::ofstream os( "test.cfg" );
    os
        << "log.file = test.log" << "\n"
        << "log.levels = info, error, debug, trace, test" << std::endl
        ;
    os.close();
    wchar_t* argv[] = { L"", L"--config-file=test.cfg" };
    int argc = sizeof( argv ) / sizeof( wchar_t* );
    ICommandLine::instance().parse_command_line( argc, argv );
    ILog::instance();

    while ( true )
    {
        log_info << "hello, world";
        log_error << "hello, world";
        log_debug << "hello, world";
        log_trace << "hello, world";
        log_test << "hello, world";

        if ( _getch() == 'q' )
        {
            break;
        }
    }

    ILog::remove();

    try
    {
        boost::filesystem::remove( "test.cfg" );
        boost::filesystem::remove( "test.log" );   
    }
    catch ( std::exception& e )
    {
        std::cout << "error: " << e.what() << std::endl;
    }
}
