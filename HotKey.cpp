#include "stdafx.h"
#include "HotKey.h"
#include "IInputSender.h"


HotKey::HotKey()
    : m_running( true ),
      m_thread_pool( 5 ),
      m_message_loop( false )
{
    m_thread = boost::thread( boost::bind( &HotKey::message_loop, this ) );
    
    while ( ! m_message_loop )
    {
        IInputSender::instance().Ctrl_Alt_Shift_key( 'M' );
        Sleep( 1 );
    }
}


HotKey::~HotKey()
{
    m_running = false;
    //IInputSender::instance().Ctrl_Alt_Shift_key( 'Q' );
    //m_thread.join();
}


IHotKey& HotKey::register_handler( IHotKeyHandler* handler, UINT fsModifiers, UINT vk, Callable callback )
{
    boost::lock_guard<boost::mutex> lock( m_mutex );

    KeyHandlerMap::iterator it = m_handlers.find( std::make_pair( fsModifiers, vk ) );

    if ( it != m_handlers.end() )
    {
        it->second[handler].push_back( callback );
        m_mutex.unlock();
    }
    else
    {
        boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
        static UINT id = 0;
        Key key( fsModifiers, vk );
        m_ids[key] = id;
        m_handlers[key][handler].push_back( callback );

        RegisterHandlerInfo info = { id, fsModifiers, vk };
        m_register_handler = info;
        m_operation_complete = false;
        IInputSender::instance().Ctrl_Alt_Shift_key( 'R' );
        id++;

        while ( ! m_operation_complete )
        {
            m_operation_condition.wait( operation_lock );
        }
    }

    return *this;
}


IHotKey& HotKey::unregister_handler( IHotKeyHandler* handler )
{
    boost::lock_guard<boost::mutex> lock( m_mutex );

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
        boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
        m_unregister_ids.swap( ids );
        m_operation_complete = false;
        IInputSender::instance().Ctrl_Alt_Shift_key( 'U' );

        while ( ! m_operation_complete )
        {
            m_operation_condition.wait( operation_lock );
        }
    }

    return *this;
}


void HotKey::clear()
{
    boost::lock_guard<boost::mutex> lock( m_mutex );
    boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
    m_handlers.clear();
    m_ids.clear();
    m_operation_complete = false;
    IInputSender::instance().Ctrl_Alt_Shift_key( 'C' );

    while ( ! m_operation_complete )
    {
        m_operation_condition.wait( operation_lock );
    }
}


void HotKey::message_loop()
{
    BOOL hotkey_m = RegisterHotKey( NULL, 0xBFFB, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'M' ); // Message Loop
    BOOL hotkey_q = RegisterHotKey( NULL, 0xBFFC, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'Q' ); // Quit
    BOOL hotkey_c = RegisterHotKey( NULL, 0xBFFD, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'C' ); // Clear
    BOOL hotkey_u = RegisterHotKey( NULL, 0xBFFE, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'U' ); // Unregister
    BOOL hotkey_r = RegisterHotKey( NULL, 0xBFFF, (MOD_CONTROL|MOD_ALT|MOD_SHIFT), 'R' ); // Register

    if ( !hotkey_m || !hotkey_q || !hotkey_c || !hotkey_u || !hotkey_r )
    {
        throw;
    }

    MSG msg = { 0 };

    while ( m_running && ( GetMessage( &msg, 0, 0, 0 ) != 0 ) )
    {
        if ( msg.message == WM_HOTKEY )
        {
            if ( 0xBFFB == msg.wParam ) // Message Loop
            {
                UnregisterHotKey( NULL, 0xBFFB );
                m_message_loop = true;
            }
            else if ( 0xBFFC == msg.wParam ) // Quit
            {
                UnregisterHotKey( NULL, 0xBFFF );
                UnregisterHotKey( NULL, 0xBFFE );
                UnregisterHotKey( NULL, 0xBFFD );
                UnregisterHotKey( NULL, 0xBFFC );

                BOOST_FOREACH( KeyIdMap::value_type& v, m_ids )
                {
                    UnregisterHotKey( NULL, v.second );
                }

                break;;
            }
            else if ( 0xBFFD == msg.wParam ) // Clear
            {
                boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
                BOOST_FOREACH( KeyIdMap::value_type& v, m_ids )
                {
                    UnregisterHotKey( NULL, v.second );
                }
                m_operation_complete = true;
                m_operation_condition.notify_one();
            }
            else if ( 0xBFFE == msg.wParam ) // Unregister
            {
                boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
                BOOST_FOREACH( int id, m_unregister_ids )
                {
                    UnregisterHotKey( NULL, id );
                }
                m_operation_complete = true;
                m_operation_condition.notify_one();
            }
            else if ( 0xBFFF == msg.wParam ) // Register
            {
                boost::unique_lock<boost::mutex> operation_lock( m_operation_mutex );
                RegisterHandlerInfo& i = m_register_handler;
                RegisterHotKey( NULL, i.id, i.modifiers, i.vk );
                m_operation_complete = true;
                m_operation_condition.notify_one();
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
