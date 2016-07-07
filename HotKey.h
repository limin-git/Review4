#pragma once
#include "IHotKey.h"


struct HotKey : IHotKey
{
public:

    HotKey();
    ~HotKey();
    virtual IHotKey& register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, HotKeyCallback callback );
    virtual IHotKey& unregister_handler( IHotKeyHandler* handler );

private:

    void message_loop();

private:

    struct RegisterHandlerInfo
    {
        IHotKeyHandler* handler;
        UINT modifiers;
        UINT vk;
        HotKeyCallback callback;
    };

    RegisterHandlerInfo m_register_handler;
    IHotKeyHandler* m_unregister_handler;

private:

    std::map< IHotKeyHandler*, std::set<size_t> > m_handlers;
    std::map<size_t, HotKeyCallback> m_callbacks;
    boost::thread* m_thread;
    bool m_running;
};
