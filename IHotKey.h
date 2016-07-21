#pragma once
#include "IHotKeyHandler.h"


struct IHotKey
{
    virtual ~IHotKey() {}
    virtual IHotKey& register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, Function callback ) = 0;
    virtual IHotKey& unregister_handler( IHotKeyHandler* handler ) = 0;
    virtual void clear() = 0;
    static IHotKey& instance();
};
