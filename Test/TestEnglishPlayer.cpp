#include "stdafx.h"
#include "TestEnglishPlayer.h"
#include "IEnglishPlayer.h"
#include "ICommandLine.h"


TestEnglishPlayer::TestEnglishPlayer()
{
    test1();
    //test2();
    //test3();
}


void TestEnglishPlayer::test1()
{
    std::ofstream os( "test.cfg" );
    os
        << "speech.path = C:\\Users\\Limin\\AppData\\Local\\Lingoes\\Translator\\user_data\\speech\\142000 | .wav" << "\n"
        ;
    os.close();
    wchar_t* argv[] = { L"", L"--config-file=test.cfg" };
    int argc = sizeof( argv ) / sizeof( wchar_t* );
    ICommandLine::instance().parse_command_line( argc, argv );

    IEnglishPlayer::instance().speak( L"Hello, World!" );

    while ( true )
    {
        std::wstring ws;
        std::getline( std::wcin, ws );
        IEnglishPlayer::instance().speak( ws );

        if ( ws == L"q" )
        {
            break;
        }
    }
}
