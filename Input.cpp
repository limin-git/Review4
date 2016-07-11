#include "stdafx.h"
#include "Input.h"


Input::Input()
    : m_thread_pool( 10 )
{
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

                    if ( e.bKeyDown != TRUE )
                    {
                        continue;
                    }

                    KeyHandlerMap::iterator it = m_key_handlers.find( std::make_pair( control_key_state( e.dwControlKeyState ), e.wVirtualKeyCode ) );

                    if ( it != m_key_handlers.end() )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& v, it->second )
                        {
                            m_thread_pool.queue_items( v.second );
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
                            m_thread_pool.queue_items( v.second );
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


IInput& Input::add_key_handler( IInputHandler* handler, UINT state, WORD vk, const Callback& callback )
{
    m_key_handlers[std::make_pair( state, vk )][handler].push_back( callback );
    return *this;
}


IInput& Input::add_key_handler( IInputHandler* handler, UINT state, WORD vk_first, WORD vk_last, const Callback& callback )
{
    for ( size_t vk = vk_first; vk <= vk_last; ++vk )
    {
        m_key_handlers[std::make_pair(state, vk)][handler].push_back( callback );
    }

    return *this;
}


IInput& Input::remove_key_handler( IInputHandler* handler )
{
    BOOST_FOREACH( KeyHandlerMap::value_type& v, m_key_handlers )
    {
        v.second.erase( handler );
    }

    return *this;
}


IInput& Input::add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback )
{
    ( 0 == event_flas ? m_mouse_button_pressed_handlers[button_state][handler] : m_other_mouse_handlers[event_flas][handler] ).push_back( callback );

    if ( 0 == event_flas )
    {
        m_mouse_button_pressed_handlers[button_state][handler].push_back( callback );
    }
    else
    {
        m_other_mouse_handlers[event_flas][handler].push_back( callback );
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


UINT Input::control_key_state( DWORD state )
{
    short shift   = ( ( state & (SHIFT_PRESSED) ) != 0 ) << 2;                          // MOD_SHIFT   = 4
    short control = ( ( state & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED) ) != 0 ) << 1;   // MOD_CONTROL = 2
    short alt     = ( ( state & (LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED) ) != 0 );          // MOD_ALT     = 1
    return (control | alt | shift );
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
