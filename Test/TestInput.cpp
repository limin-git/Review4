#include "stdafx.h"
#include "TestInput.h"
#include "IInput.h"


TestInput::TestInput()
{
    test1();
    //test2();
    //test3();
}


void TestInput::test1()
{
    struct KeyHandler : IInputHandler
    {
        void chars()
        {
            std::cout << "chars" << std::endl;
        }

        void arrows()
        {
            std::cout << "arrows" << std::endl;
        }

        void Fn()
        {
            std::cout << "Fn" << std::endl;
        }

        void enter()
        {
            std::cout << "Enter" << std::endl;
        }

        void control_A()
        {
            std::cout << "Control + A" << std::endl;
        }

        void shift_B()
        {
            std::cout << "Shift + B" << std::endl;
        }

        void alt_C()
        {
            std::cout << "Alt + C" << std::endl;
        }

        void control_alt_D()
        {
            std::cout << "Control + Alt + D" << std::endl;
        }

    } kh;

    IInput::instance()
        .add_key_handler( &kh, 0, VK_LEFT,  boost::bind( &KeyHandler::arrows, &kh ) )
        .add_key_handler( &kh, 0, VK_F1, boost::bind( &KeyHandler::Fn, &kh ) )
        .add_key_handler( &kh, 0, VK_RETURN, boost::bind( &KeyHandler::enter, &kh ) )
        .add_key_handler( &kh, MOD_CONTROL, 'A', boost::bind( &KeyHandler::control_A, &kh ) )
        .add_key_handler( &kh, MOD_SHIFT, 'B', boost::bind( &KeyHandler::shift_B, &kh ) )
        .add_key_handler( &kh, MOD_ALT, 'C', boost::bind( &KeyHandler::alt_C, &kh ) )
        .add_key_handler( &kh, (MOD_CONTROL|MOD_ALT), 'D', boost::bind( &KeyHandler::control_alt_D, &kh ) )
        .run()
        ;
}


void TestInput::test2()
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

        void wheeled()
        {
            std::cout << "wheeled" << std::endl;
        }
    } mh;

    IInput::instance()
        .add_mouse_handler( &mh, 0, FROM_LEFT_1ST_BUTTON_PRESSED, boost::bind( &MouseHandler::left_button_pressed, &mh ) )
        .add_mouse_handler( &mh, 0, RIGHTMOST_BUTTON_PRESSED, boost::bind( &MouseHandler::right_button_pressed, &mh ) )
        .add_mouse_handler( &mh, DOUBLE_CLICK, 0, boost::bind( &MouseHandler::double_click, &mh ) )
        .add_mouse_handler( &mh, MOUSE_WHEELED, 0, boost::bind( &MouseHandler::wheeled, &mh ) )
        .run();
}


void TestInput::test3()
{
    struct KeyHandler : IInputHandler
    {
        void chars()
        {
            std::cout << "chars" << std::endl;
        }

        void arrows()
        {
            std::cout << "arrows" << std::endl;
        }

        void Fn()
        {
            std::cout << "Fn" << std::endl;
        }

        void enter()
        {
            std::cout << "Enter" << std::endl;
        }
    } kh;

    IInput::instance()
        .add_key_handler( &kh, 0, VK_LEFT,  boost::bind( &KeyHandler::arrows, &kh ) )
        .add_key_handler( &kh, 0, VK_F1, boost::bind( &KeyHandler::Fn, &kh ) )
        .add_key_handler( &kh, 0, VK_RETURN, boost::bind( &KeyHandler::enter, &kh ) );

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

        void wheeled()
        {
            std::cout << "wheeled" << std::endl;
        }
    } mh;

    IInput::instance()
        .add_mouse_handler( &mh, 0, FROM_LEFT_1ST_BUTTON_PRESSED, boost::bind( &MouseHandler::left_button_pressed, &mh ) )
        .add_mouse_handler( &mh, 0, RIGHTMOST_BUTTON_PRESSED, boost::bind( &MouseHandler::right_button_pressed, &mh ) )
        .add_mouse_handler( &mh, DOUBLE_CLICK, 0, boost::bind( &MouseHandler::double_click, &mh ) )
        .add_mouse_handler( &mh, MOUSE_WHEELED, 0, boost::bind( &MouseHandler::wheeled, &mh ) );

    IInput::instance().run();
}
