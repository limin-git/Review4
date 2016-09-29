#pragma once
#include "IInputHandler.h"


struct IInput
{
    virtual ~IInput() {}
    virtual void run() = 0;
    virtual void terminate() = 0;
    virtual IInput& register_handler( IInputHandler* handler, UINT control_state, WORD vk, const Callback& callback ) = 0;
    virtual IInput& register_handler( IInputHandler* handler, UINT control_state, WORD vk_first, WORD vk_last, const Callback& callback ) = 0;
    virtual IInput& unregister_handler( IInputHandler* handler ) = 0;
    virtual IInput& register_mouse_handler( IInputHandler* handler, DWORD event_flas, DWORD button_state, const Callback& callback ) = 0;
    virtual IInput& unregister_mouse_handler( IInputHandler* handler ) = 0;
    virtual IInput& clear() = 0;
    static IInput& instance();
};
