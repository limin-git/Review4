#include "stdafx.h"
#include "IInput.h"


struct Input : IInput
{
public:

    Input();
    virtual void run();
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code, const Callback& callback );
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code_first, WORD virtual_key_code_last, const Callback& callback );
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, const std::vector<WORD>& virtual_key_code_list, const Callback& callback );
    virtual IInput& remove_key_handler( IInputHandler* handler );
    virtual IInput& add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback );
    virtual IInput& remove_mouse_handler( IInputHandler* handler );

private:

    typedef std::map<IInputHandler*, Callback> CallbackMap;
    typedef std::map<WORD, CallbackMap> KeyHandlerMap;
    std::vector<KeyHandlerMap> m_key_handlers;
    typedef std::map<DWORD, CallbackMap> MouseHandlerMap;
    MouseHandlerMap m_mouse_button_pressed_handlers;
    MouseHandlerMap m_other_mouse_handlers;
};