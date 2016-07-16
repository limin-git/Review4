#include "stdafx.h"
#include "HotKey.h"
#include "IInputSender.h"


HotKey::HotKey()
    : m_running( true ),
      m_thread_pool( 5 )
{
    m_thread = boost::thread( boost::bind( &HotKey::message_loop, this ) );
    m_mutex.lock();
    Sleep(20);
}


HotKey::~HotKey()
{
    m_running = false;
    //IInputSender::instance().Ctrl_Alt_Shift_key( 'Q' );
    //m_thread.join();
}


IHotKey& HotKey::register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, Callable callback )
{
    m_mutex.lock();

    KeyHandlerMap::iterator it = m_handlers.find( std::make_pair( fsModifiers, vk ) );

    if ( it != m_handlers.end() )
    {
        it->second[handler].push_back( callback );
        m_mutex.unlock();
    }
    else
    {
        static UINT id = 0;
        Key key( fsModifiers, vk );
        m_ids[key] = id;
        m_handlers[key][handler].push_back( callback );

        RegisterHandlerInfo info = { id, fsModifiers, vk };
        m_register_handler = info;
        IInputSender::instance().Ctrl_Alt_Shift_key( 'R' );
        id++;
    }

    return *this;
}


IHotKey& HotKey::unregister_handler( IHotKeyHandler* handler )
{
    m_mutex.lock();

    std::set<UINT> ids;

    for ( KeyHandlerMap::iterator it = m_handlers.begin(); it != m_handlers.end(); NULL )
    {
        it->second.erase( handler );

        if ( it->second.empty() )
        {
            Key key = it->first;
            UINT id = m_ids[key];
            m_ids.erase( key );
            ids.insert( id );
            m_handlers.erase( it++ );
        }
        else
        {
            it++;
        }
    }

    if ( ! ids.empty() )
    {
        m_unregister_ids.swap( ids );
        IInputSender::instance().Ctrl_Alt_Shift_key( 'U' );
    }
    else
    {
        m_mutex.unlock();
    }

    return *this;
}


void HotKey::message_loop()
{
    RegisterHotKey( NULL, 0xBFFD, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'R' ); // Register
    RegisterHotKey( NULL, 0xBFFE, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'U' ); // Unregister
    RegisterHotKey( NULL, 0xBFFF, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'Q' ); // Quit
    
    m_mutex.unlock();

    MSG msg = { 0 };

    while ( m_running && ( GetMessage( &msg, 0, 0, 0 ) != 0 ) )
    {
        if ( msg.message == WM_HOTKEY )
        {
            if ( 0xBFFD == msg.wParam ) // Register
            {
                RegisterHandlerInfo i = m_register_handler;
                m_mutex.unlock();
                RegisterHotKey( NULL, i.id, i.modifiers, i.vk );
            }
            if ( 0xBFFE == msg.wParam ) // Unregister
            {
                std::set<UINT> ids;
                ids.swap( m_unregister_ids );
                m_mutex.unlock();

                BOOST_FOREACH( int id, ids )
                {
                    UnregisterHotKey( NULL, id );
                }
            }
            else if ( 0xBFFF == msg.wParam ) // Quit
            {
                UnregisterHotKey( NULL, 0xBFFD );
                UnregisterHotKey( NULL, 0xBFFE );
                UnregisterHotKey( NULL, 0xBFFF );

                BOOST_FOREACH( KeyIdMap::value_type& v, m_ids )
                {
                    UnregisterHotKey( NULL, v.second );
                }

                return;
            }
            else
            {
                BOOST_FOREACH( KeyIdMap::value_type& v, m_ids )
                {
                    if ( v.second == msg.wParam )
                    {
                        BOOST_FOREACH( CallbackMap::value_type& vc, m_handlers[v.first] )
                        {
                            m_thread_pool.queue_items( vc.second );
                        }
                    }
                }
            }
        }
    }
}
