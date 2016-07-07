#include "stdafx.h"
#include "TestEnglishChineseExampleText.h"
#include "EnglishChineseExampleText.h"
#include "ICommandLine.h"


TestEnglishChineseExampleText::TestEnglishChineseExampleText()
{
    test1();
}


void TestEnglishChineseExampleText::test1()
{
    {
        std::ofstream os( "test.ece" );
        os
            << "hello"          << "\n"
            << "���"           << "\n"
            << "hello, world!"  << "\n"
            << "���磬 ��ã�"  << "\n\n"
            << "morning"        << "\n"
            << "����"           << "\n"
            << "good morning!"  << "\n"
            << "���Ϻã�"       << "\n\n"
            ;
        os.close();
    }

    {
        std::ofstream os( "test.cfg" );
        os
            << "speech.path = C:\\Users\\Limin\\AppData\\Local\\Lingoes\\Translator\\user_data\\speech\\142000 | .wav" << "\n"
            ;
        os.close();
        wchar_t* argv[] = { L"", L"--config-file=test.cfg" };
        int argc = sizeof( argv ) / sizeof( wchar_t* );
        ICommandLine::instance().parse_command_line( argc, argv );
    }

    EnglishChineseExampleText text( "test.ece" );
    const KeyList& keys = text.keys();

    BOOST_FOREACH( size_t key, keys )
    {
        ISlideshowPtr slid = text.slideshow( key );

        for ( bool finished = slid->show(); !finished; )
        {
            _getch();
            finished = slid->show();
        }

        _getch();
    }

    _getch();
}
