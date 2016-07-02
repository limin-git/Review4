#include "stdafx.h"
#include "TestSpeech.h"
#include "ISpeech.h"


TestSpeech::TestSpeech()
{
    test1();
}


void TestSpeech::test1()
{
    ISpeech::instance().speak( L"hello, world" );

    while ( true )
    {
        std::wstring ws;
        std::getline( std::wcin, ws );
        ISpeech::instance().speak( ws );

        if ( _getch() == 'q' )
        {
            break;
        }
    }
}
