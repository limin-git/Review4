#pragma once
#include "IHotKey.h"
#include "ThreadPool.h"


struct HotKey : IHotKey
{
public:

    HotKey();
    ~HotKey();
    virtual IHotKey& register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, Function callback );
    virtual IHotKey& unregister_handler( IHotKeyHandler* handler );
    virtual void clear();

private:

    void message_loop();
    bool completed() const { return m_operation_complete; }

private:

    struct RegisterHandlerInfo
    {
        UINT id;
        UINT modifiers;
        UINT vk;
    };

    RegisterHandlerInfo m_register_handler;
    std::set<UINT> m_unregister_ids;

private:

    typedef std::vector<Function> CallableList;
    typedef std::map<IHotKeyHandler*, CallableList> CallbackMap;
    typedef std::pair<UINT, UINT> Key; // modifier, vk
    typedef std::map<Key, CallbackMap> KeyHandlerMap;
    typedef std::map<Key, UINT> KeyIdMap;
    KeyHandlerMap m_handlers;
    KeyIdMap m_ids;
    boost::thread m_thread;
    bool m_running;
    ThreadPool<2> m_thread_pool;
    boost::mutex m_mutex;
    boost::mutex m_operation_mutex;
    boost::condition_variable m_operation_condition;
    bool m_operation_complete;
    volatile bool m_message_loop;
    std::set<UINT> m_failed_ids;
};
