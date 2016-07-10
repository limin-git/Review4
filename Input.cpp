#include "stdafx.h"
#include "Input.h"


Input::Input()
{
    m_key_handlers.resize( 2 );
}


void Input::run()
{
    HANDLE cin = GetStdHandle(STD_INPUT_HANDLE);
    const size_t size = 1024;
    INPUT_RECORD input_buffer[size];
    DWORD num_read = 0;

    SetConsoleMode( cin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT );

    while ( true )
    {
        ReadConsoleInput( cin, input_buffer, size, &num_read );

        BOOST_FOREACH( INPUT_RECORD& input, input_buffer )
        {
            switch( input.EventType )
            {
            case KEY_EVENT:
                {
                    KEY_EVENT_RECORD& e = input.Event.KeyEvent;

                    // For release version
                    if ( ( e.wRepeatCount != 1 ) || ( 0 == e.wVirtualKeyCode ) || ( 1 < e.bKeyDown ) )
                    {
                        continue;
                    }

                    if ( VK_ESCAPE == e.wVirtualKeyCode )
                    {
                        return;
                    }

                    KeyHandlerMap::iterator it = m_key_handlers[e.bKeyDown != 0].find( e.wVirtualKeyCode );

                    if ( it != m_key_handlers[e.bKeyDown != 0].end() )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& v, it->second )
                        {
                            m_processor.queue_item( boost::bind( &Input::callback_thread, this, v.second ) );
                        }
                    }
                }
                break;

            case MOUSE_EVENT:
                {
                    MOUSE_EVENT_RECORD& e = input.Event.MouseEvent;

                    if ( MOUSE_MOVED == e.dwEventFlags )
                    {
                        continue;
                    }

                    // For release version
                    if ( ( FROM_LEFT_4TH_BUTTON_PRESSED < e.dwButtonState ) || ( MOUSE_HWHEELED < e.dwEventFlags ) || ( 0x01FF < e.dwControlKeyState ) )
                    {
                        continue;
                    }

                    MouseHandlerMap& mouse_handlers = ( 0 == e.dwEventFlags ? m_mouse_button_pressed_handlers : m_other_mouse_handlers );
                    MouseHandlerMap::iterator it = mouse_handlers.find( 0 == e.dwEventFlags ? e.dwButtonState : e.dwEventFlags );

                    if ( it != mouse_handlers.end() )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& v, it->second )
                        {
                            m_processor.queue_item( boost::bind( &Input::callback_thread, this, v.second ) );
                        }
                    }
                }
                break;

            case WINDOW_BUFFER_SIZE_EVENT:
            case MENU_EVENT:
            case FOCUS_EVENT:
            default:
                break;
            }
        }
    }
}


IInput& Input::add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code, const boost::function<void()>& callback )
{
    m_key_handlers[key_down][virtual_key_code][handler] = callback;
    return *this;
}


IInput& Input::add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code_first, WORD virtual_key_code_last, const Callback& callback )
{
    for ( size_t virtual_key_code = virtual_key_code_first; virtual_key_code <= virtual_key_code_last; ++virtual_key_code )
    {
        m_key_handlers[key_down][virtual_key_code][handler] = callback;
    }

    return *this;
}


IInput& Input::add_key_handler( IInputHandler* handler, bool key_down, const std::vector<WORD>& virtual_key_code_list, const Callback& callback )
{
    BOOST_FOREACH( WORD virtual_key_code, virtual_key_code_list )
    {
        m_key_handlers[key_down][virtual_key_code][handler] = callback;
    }

    return *this;
}


IInput& Input::remove_key_handler( IInputHandler* handler )
{
    BOOST_FOREACH( KeyHandlerMap& key_handler, m_key_handlers)
    {
        BOOST_FOREACH( KeyHandlerMap::value_type& v, key_handler )
        {
            v.second.erase( handler );
        }
    }

    return *this;
}


IInput& Input::add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback )
{
    ( 0 == event_flas ? m_mouse_button_pressed_handlers[button_state][handler] : m_other_mouse_handlers[event_flas][handler] ) = callback;

    if ( 0 == event_flas )
    {
        m_mouse_button_pressed_handlers[button_state][handler] = callback;
    }
    else
    {
        m_other_mouse_handlers[event_flas][handler] = callback;
    }

    return *this;
}


IInput& Input::remove_mouse_handler( IInputHandler* handler )
{
    BOOST_FOREACH( MouseHandlerMap::value_type& v, m_mouse_button_pressed_handlers )
    {
        v.second.erase( handler );
    }

    BOOST_FOREACH( MouseHandlerMap::value_type& v, m_other_mouse_handlers )
    {
        v.second.erase( handler );
    }

    return *this;
}


void Input::callback_thread( const Callback& callback )
{
    callback();
}


void Input::debug_print_key_event( const KEY_EVENT_RECORD& e )
{
    static size_t i = 0;
    std::stringstream strm;
    strm
        << std::setw(3) << std::setfill('0') << ++i << "\n"
        << "bKeyDown=" << e.bKeyDown << "\n"
        << "wRepeatCount=" << e.wRepeatCount << "\n"
        << "wVirtualKeyCode=" << std::hex << e.wVirtualKeyCode << "\n"
        << "wVirtualScanCode=" << std::hex << e.wVirtualScanCode << "\n"
        << "wRepeatCount=" << e.wRepeatCount << "\n"
        << "uChar.AsciiChar=" << e.uChar.AsciiChar << "\n\n"
        ;
    std::cout << strm.str() << std::endl;
}


void Input::debug_print_mouse_event( const MOUSE_EVENT_RECORD& e )
{
    static size_t i = 0;
    std::stringstream strm;
    strm
        << std::setw(3) << std::setfill('0') << ++i << "\n"
        << "dwButtonState=" << std::hex << e.dwButtonState << "\n"
        << "dwControlKeyState=" << std::hex << e.dwControlKeyState << "\n"
        << "dwEventFlags=" << std::hex << e.dwEventFlags << "\n\n"
        ;
    std::cout << strm.str() << std::endl;
}
