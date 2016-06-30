#include "stdafx.h"
#include "Input.h"


Input::Input()
{
    m_key_handlers.resize( 2 );
}


void Input::run()
{
    HANDLE cout = GetStdHandle(STD_INPUT_HANDLE);
    const size_t size = 128;
    INPUT_RECORD input_buffer[size];
    DWORD num_read = 0;

    SetConsoleMode( cout, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT );

    while ( true )
    {
        ReadConsoleInput( cout, input_buffer, size, &num_read );

        BOOST_FOREACH( INPUT_RECORD& input, input_buffer )
        {
            switch( input.EventType )
            {
            case KEY_EVENT:
                {
                    KEY_EVENT_RECORD& e = input.Event.KeyEvent;

                    if ( VK_ESCAPE == e.wVirtualKeyCode )
                    {
                        return;
                    }

                    KeyHandlerMap::iterator it = m_key_handlers[e.bKeyDown].find( e.wVirtualKeyCode );

                    if ( it != m_key_handlers[e.bKeyDown].end() )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& v, it->second )
                        {
                            v.second();
                        }
                    }
                }
                break;
            case MOUSE_EVENT:
                {
                    MOUSE_EVENT_RECORD& e = input.Event.MouseEvent;

                    CallbackMap* callbacks = NULL;

                    if ( 0 == e.dwEventFlags )
                    {
                        MouseHandlerMap::iterator it = m_mouse_button_pressed_handlers.find( e.dwButtonState );

                        if ( it != m_mouse_button_pressed_handlers.end() )
                        {
                            callbacks = &( it->second );
                        }
                    }
                    else
                    {
                        MouseHandlerMap::iterator it = m_other_mouse_handlers.find( e.dwEventFlags );

                        if ( it != m_other_mouse_handlers.end() )
                        {
                            callbacks = &( it->second );
                        }
                    }

                    if ( callbacks != NULL )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& v, *callbacks )
                        {
                            v.second();
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
    }
}


void Input::add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code, const boost::function<void()>& callback )
{
    m_key_handlers[key_down][virtual_key_code][handler] = callback;
}


void Input::add_key_handler( IInputHandler* handler, bool key_down, const std::vector<WORD>& virtual_key_code_list, const Callback& callback )
{
    BOOST_FOREACH( WORD virtual_key_code, virtual_key_code_list )
    {
        m_key_handlers[key_down][virtual_key_code][handler] = callback;
    }
}


void Input::remove_key_handler( IInputHandler* handler )
{
    BOOST_FOREACH( KeyHandlerMap& key_handler, m_key_handlers)
    {
        BOOST_FOREACH( KeyHandlerMap::value_type& v, key_handler )
        {
            v.second.erase( handler );
        }
    }
}


void Input::add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback )
{
    if ( 0 == event_flas )
    {
        m_mouse_button_pressed_handlers[button_state][handler] = callback;
    }
    else
    {
        m_other_mouse_handlers[event_flas][handler] = callback;
    }
}


void Input::remove_mouse_handler( IInputHandler* handler )
{
    BOOST_FOREACH( MouseHandlerMap::value_type& v, m_mouse_button_pressed_handlers )
    {
        v.second.erase( handler );
    }

    BOOST_FOREACH( MouseHandlerMap::value_type& v, m_other_mouse_handlers )
    {
        v.second.erase( handler );
    }
}
