#include "stdafx.h"
#include "HotKey.h"
#include "IInputSender.h"


HotKey::HotKey()
    : m_thread( NULL ),
      m_running( true ),
      m_unregister_handler( NULL )
{
    m_thread = new boost::thread( boost::bind( &HotKey::message_loop, this ) );
    Sleep( 20 );
}


HotKey::~HotKey()
{
    m_running = false;
    IInputSender::instance().key( 'Q', true, true, true );
    m_thread->join();
    delete m_thread;
}


IHotKey& HotKey::register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, HotKeyCallback callback )
{
    RegisterHandlerInfo info = { handler, fsModifiers, vk, callback };
    m_register_handler = info;
    IInputSender::instance().key( 'R', true, true, true );
    Sleep( 5 );
    return *this;
}


IHotKey& HotKey::unregister_handler( IHotKeyHandler* handler )
{
    m_unregister_handler = handler;
    IInputSender::instance().key( 'U', true, true, true );
    return *this;
}


void HotKey::message_loop()
{
    RegisterHotKey( NULL, 0xBFFD, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'R' ); // Register
    RegisterHotKey( NULL, 0xBFFE, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'U' ); // Unregister
    RegisterHotKey( NULL, 0xBFFF, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'Q' ); // Quit

    MSG msg = { 0 };

    while ( m_running && ( GetMessage( &msg, 0, 0, 0 ) != 0 ) )
    {
        if ( msg.message == WM_HOTKEY )
        {
            if ( 0xBFFD == msg.wParam ) // Register
            {
                RegisterHandlerInfo& i = m_register_handler;
                int id = i.handler->unique_id();
                m_handlers[i.handler].insert( id );
                m_callbacks[id] = i.callback;
                RegisterHotKey( NULL, id, i.modifiers, i.vk );
            }
            if ( 0xBFFE == msg.wParam ) // Unregister
            {
                if ( m_unregister_handler )
                {
                    BOOST_FOREACH( int id, m_handlers[m_unregister_handler] )
                    {
                        UnregisterHotKey( NULL, id );
                        m_callbacks.erase( id );
                    }

                    m_handlers.erase( m_unregister_handler );
                    m_unregister_handler = NULL;
                }
            }
            else if ( 0xBFFF == msg.wParam ) // Quit
            {
                UnregisterHotKey( NULL, 0xBFFD );
                UnregisterHotKey( NULL, 0xBFFE );
                UnregisterHotKey( NULL, 0xBFFF );

                for ( std::map<size_t, HotKeyCallback>::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it )
                {
                    UnregisterHotKey( NULL, it->first );
                }

                return;
            }
            else
            {
                std::map<size_t, HotKeyCallback>::iterator it = m_callbacks.find( msg.wParam );

                if ( it != m_callbacks.end() )
                {
                    (it->second)();
                }
            }
        }
    }
}
