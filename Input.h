#include "stdafx.h"
#include "IInput.h"
#include "ThreadPool.h"


struct Input : IInput
{
public:

    virtual void run();
    virtual IInput& register_handler( IInputHandler* handler, UINT state, WORD vk, const Callback& callback );
    virtual IInput& register_handler( IInputHandler* handler, UINT state, WORD vk_first, WORD vk_last, const Callback& callback );
    virtual IInput& unregister_handler( IInputHandler* handler );
    virtual IInput& register_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback );
    virtual IInput& unregister_mouse_handler( IInputHandler* handler );
    virtual IInput& clear();

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
    ThreadPool<1> m_thread_pool;
    boost::mutex m_mutex;
};
