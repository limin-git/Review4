#pragma once
#include "IHotKeyHandler.h"


struct IHotKey
{
    virtual ~IHotKey() {}
    virtual void register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, HotKeyCallback callback ) = 0;
    virtual void unregister_handler( IHotKeyHandler* handler ) = 0;
    static IHotKey& instance();
    static void remove();
};
