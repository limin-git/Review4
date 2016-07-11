#pragma once
#include "IHotKey.h"
#include "IInputSender.h"
#include "Singleton.h"
#include "ThreadPool.h"


struct HotKey : IHotKey
{
public:

    HotKey();
    ~HotKey();
    virtual IHotKey& register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, Callable callback );
    virtual IHotKey& unregister_handler( IHotKeyHandler* handler );

private:

    void message_loop();

private:

    struct RegisterHandlerInfo
    {
        UINT id;
        UINT modifiers;
        UINT vk;
    };

    RegisterHandlerInfo m_register_handler;
    std::set<UINT> m_unregister_ids;
    boost::mutex m_mutex;

private:

    typedef std::vector<Callable> CallableList;
    typedef std::map<IHotKeyHandler*, CallableList> CallbackMap;
    typedef std::pair<UINT, UINT> Key; // modifier, vk
    typedef std::map<Key, CallbackMap> KeyHandlerMap;
    typedef std::map<Key, size_t> KeyIdMap;
    KeyHandlerMap m_handlers;
    KeyIdMap m_ids;
    boost::thread m_thread;
    bool m_running;
    Singleton<IInputSender> m_input_sender;
    ThreadPool m_thread_pool;
};
