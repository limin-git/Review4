#include "stdafx.h"
#include "TestInput.h"
#include "IInput.h"


TestInput::TestInput()
{
    //test1();
    //test2();
    test3();
}


void TestInput::test1()
{
    IInput::instance().run();
}


void TestInput::test2()
{
    struct KeyHandler : IInputHandler
    {
        void handle_chars()
        {
            std::cout << "chars" << std::endl;
        }
    };

    KeyHandler h;
    Callback callback( boost::bind( &KeyHandler::handle_chars, &h ) );

    for ( unsigned char ch = 'A'; ch <= 'Z'; ++ch  )
    {
        IInput::instance().add_key_handler( &h, false, ch, callback );
    }

    IInput::instance().run();
}


void TestInput::test3()
{
    struct MouseHandler : IInputHandler
    {
        void left_button_pressed()
        {
            std::cout << "left" << std::endl;
        }

        void right_button_pressed()
        {
            std::cout << "right" << std::endl;
        }

        void double_click()
        {
            std::cout << "double click" << std::endl;
        }
    };

    MouseHandler h;
    IInput::instance().add_mouse_handler( &h, 0, FROM_LEFT_1ST_BUTTON_PRESSED, boost::bind( &MouseHandler::left_button_pressed, &h ) );
    IInput::instance().add_mouse_handler( &h, 0, RIGHTMOST_BUTTON_PRESSED, boost::bind( &MouseHandler::right_button_pressed, &h ) );
    IInput::instance().add_mouse_handler( &h, 1, DOUBLE_CLICK, boost::bind( &MouseHandler::double_click, &h ) );

    IInput::instance().run();
}
