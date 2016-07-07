#include "stdafx.h"
#include "TestText.h"
#include "ICommandLine.h"
#include "IText.h"


TestText::TestText()
{
    test1();
}


void TestText::test1()
{
    {
        std::ofstream os( "test.ece" );
        os
            << "hello"          << "\n"
            << "你好"           << "\n"
            << "hello, world!"  << "\n"
            << "世界， 你好！"  << "\n\n"
            << "morning"        << "\n"
            << "早上"           << "\n"
            << "good morning!"  << "\n"
            << "早上好！"       << "\n\n"
            ;
        os.close();
    }

    {
        std::ofstream os( "test.cfg" );
        os
            << "file.name = test.ece" << "\n"
            << "speech.path = C:\\Users\\Limin\\AppData\\Local\\Lingoes\\Translator\\user_data\\speech\\142000 | .wav" << "\n"
            ;
        os.close();
        wchar_t* argv[] = { L"", L"--config-file=test.cfg" };
        int argc = sizeof( argv ) / sizeof( wchar_t* );
        ICommandLine::instance().parse_command_line( argc, argv );
    }

    IText& text = IText::instance();
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
