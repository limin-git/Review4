#include "stdafx.h"
#include "IInput.h"
#include "ThreadPool.h"


struct Input : IInput
{
public:

    Input();
    virtual void run();
    virtual IInput& add_key_handler( IInputHandler* handler, UINT state, WORD vk, const Callback& callback );
    virtual IInput& add_key_handler( IInputHandler* handler, UINT state, WORD vk_first, WORD vk_last, const Callback& callback );
    virtual IInput& remove_key_handler( IInputHandler* handler );
    virtual IInput& add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback );
    virtual IInput& remove_mouse_handler( IInputHandler* handler );

private:

    UINT control_key_state( DWORD state );

private:

    void debug_print_key_event( const KEY_EVENT_RECORD& e );
    void debug_print_mouse_event( const MOUSE_EVENT_RECORD& e );

private:

    typedef std::map<IInputHandler*, CallbackList> CallbackMap;
    typedef std::map<std::pair<UINT, WORD>, CallbackMap> KeyHandlerMap;
    KeyHandlerMap m_key_handlers;
    typedef std::map<DWORD, CallbackMap> MouseHandlerMap;
    MouseHandlerMap m_mouse_button_pressed_handlers;
    MouseHandlerMap m_other_mouse_handlers;
    ThreadPool m_thread_pool;
};
