#pragma once
#include "IInputHandler.h"


struct IInput
{
    virtual ~IInput() {}
    virtual void run() = 0;
    virtual IInput& add_key_handler( IInputHandler* handler, UINT control_state, WORD vk, const Callback& callback ) = 0;
    virtual IInput& add_key_handler( IInputHandler* handler, UINT control_state, WORD vk_first, WORD vk_last, const Callback& callback ) = 0;
    virtual IInput& remove_key_handler( IInputHandler* handler ) = 0;
    virtual IInput& add_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback ) = 0;
    virtual IInput& remove_mouse_handler( IInputHandler* handler ) = 0;
    static IInput& instance();
    static void remove();
};
