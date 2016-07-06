#include "stdafx.h"
#include "HotKey.h"


HotKey::HotKey()
    : m_thread( NULL ),
      m_running( true ),
      m_hwnd( NULL )
{
    //m_hwnd = GetActiveWindow();
    m_hwnd = GetConsoleWindow();
}


HotKey::~HotKey()
{
    m_running = false;

    if ( m_thread )
    {
        // TODO: how to trigger the message loop ? 
        //m_thread->join();
        delete m_thread;
        m_thread = NULL;
    }
}


void HotKey::register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, HotKeyCallback callback )
{
    if ( NULL == m_thread )
    {
        m_thread = new boost::thread( boost::bind( &HotKey::message_loop, this ) );
    }

    int id = handler->unique_id();
    m_handlers[handler].insert( id );
    m_callbacks[id] = callback;
    RegisterHotKey( m_hwnd, id, fsModifiers, vk );
}


void HotKey::unregister_handler( IHotKeyHandler* handler )
{
    BOOST_FOREACH( int id, m_handlers[handler] )
    {
        UnregisterHotKey( m_hwnd, id );
        m_callbacks.erase( id );
    }

    m_handlers.erase( handler );
}


void HotKey::message_loop()
{
    MSG msg = { 0 };

    while ( m_running && ( GetMessage( &msg, 0, 0, 0 ) != 0 ) )
    {
        if ( msg.message == WM_HOTKEY )
        {
            std::map<size_t, HotKeyCallback>::iterator it = m_callbacks.find( msg.wParam );

            if ( it != m_callbacks.end() )
            {
                (it->second)();
            }
        }
    }
}


void HotKey::test()
{
    // TODO
    //BOOST_FOREACH( int id, m_handlers[handler] )
    //{
    //    RegisterHotKey(hConsole, 2, 0/* | MOD_NOREPEAT*/,VK_BROWSER_BACK);// Ctrl + B
    //    RegisterHotKey(hConsole, 3, 0 /*| MOD_NOREPEAT*/,VK_BROWSER_FORWARD);    // Alt + A
    //}
}
