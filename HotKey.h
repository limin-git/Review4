#pragma once
#include "IHotKey.h"


struct HotKey : IHotKey
{
public:

    HotKey();
    ~HotKey();
    virtual void register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, HotKeyCallback callback );
    virtual void unregister_handler( IHotKeyHandler* handler );

private:

    void message_loop();

private:

    std::map< IHotKeyHandler*, std::set<size_t> > m_handlers;
    std::map<size_t, HotKeyCallback> m_callbacks;
    HWND m_hwnd;
    boost::thread* m_thread;
    bool m_running;
};
