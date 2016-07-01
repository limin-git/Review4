#pragma once
#include "IInputHandler.h"


struct IInput
{
    virtual ~IInput() {}
    virtual void run() = 0;
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code, const Callback& callback ) = 0;
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, WORD virtual_key_code_beg, WORD virtual_key_code_end, const Callback& callback ) = 0;
    virtual IInput& add_key_handler( IInputHandler* handler, bool key_down, const std::vector<WORD>& virtual_key_code_list, const Callback& callback ) = 0;
    virtual IInput& remove_key_handler( IInputHandler* handler ) = 0;
    virtual IInput& add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback ) = 0;
    virtual IInput& remove_mouse_handler( IInputHandler* handler ) = 0;
    static IInput& instance();
    static void remove();
};
