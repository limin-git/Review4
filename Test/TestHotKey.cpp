#include "stdafx.h"
#include "TestHotKey.h"
#include "IHotKey.h"


TestHotKey::TestHotKey()
{
    test1();
    //test2();
}


void TestHotKey::test1()
{
    struct HotKeyHandler : IHotKeyHandler
    {
        void handle_browser_back()
        {
            std::cout << "browser back" << std::endl;
        }

        void handle_browser_forward()
        {
            std::cout << "browser forward" << std::endl;
        }
    };

    HotKeyHandler h;
    IHotKey::instance().register_handler( &h, 0, 'A', boost::bind( &HotKeyHandler::handle_browser_back, &h ) );
    IHotKey::instance().register_handler( &h, 0, 'B', boost::bind( &HotKeyHandler::handle_browser_forward, &h ) );
    while ( true )
    {
        if ( 'q' == _getch() ) break;;
    }
}
