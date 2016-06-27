#include "stdafx.h"
#include "DirectoryChangeMonitorThread.h"
#include "FileSystem.h"


DirectoryChangeMonitorThread::DirectoryChangeMonitorThread( const fs::path& file_path, IDirectoryChangeHandler* handler )
{
    m_dir = file_path.parent_path();
    m_file_handler_map[file_path].insert( handler );
    m_file_time_map[file_path] = boost::filesystem::last_write_time( file_path );
    m_running = true;
    m_handle = NULL;
}


DirectoryChangeMonitorThread::~DirectoryChangeMonitorThread();
{
    terminate();
}


void DirectoryChangeMonitorThread::terminate()
{
    m_running = false;
    FindCloseChangeNotification( m_handle );
}


void DirectoryChangeMonitorThread::operator()()
{
    m_handle = ::FindFirstChangeNotification( m_dir.wstring().c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE );

    if ( INVALID_HANDLE_VALUE == m_handle )
    {
        return;
    }

    while ( m_running )
    {
        DWORD status = ::WaitForSingleObject( m_handle, INFINITE );

        if ( WAIT_OBJECT_0 == status )
        {
            if ( !m_running )
            {
                return;
            }

            notify_handlers();

            if ( ::FindNextChangeNotification( m_handle ) == FALSE )
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
}


void DirectoryChangeMonitorThread::notify_handlers()
{
    BOOST_FOREACH( FileHandlerMap::value_type& v, m_file_handler_map )
    {
        const fs::path& file = v.first;
        FileChangeHandlerList& handlers = v.second;

        std::time_t t = boost::filesystem::last_write_time( file );

        if ( t != m_file_time_map[file] )
        {
            BOOST_FOREACH( IDirectoryChangeHandler* handler, handlers )
            {
                handler->last_write_time_changed( file );
            }
        }
    }
}


bool DirectoryChangeMonitorThread::merge_child( const DirectoryChangeMonitorThread& rhs )
{
    if ( !FileSystem::is_parent_path( m_dir, rhs.m_dir ) )
    {
        return false;
    }

    BOOST_FOREACH( const FileHandlerMap::value_type& v, rhs.m_file_handler_map )
    {
        m_file_handler_map.insert( v );
    }

    BOOST_FOREACH( const FileTimeMap::value_type& v, rhs.m_file_time_map )
    {
        m_file_time_map.insert( v );
    }

    return true;
}


bool DirectoryChangeMonitorThread::is_monitored( const fs::path& file_path )
{
    return ( m_file_time_map.find( file_path ) != m_file_time_map.end() );
}


void DirectoryChangeMonitorThread::remove_handler( const fs::path& file_path, IDirectoryChangeHandler* handler )
{
    if ( is_monitored( file_path ) )
    {
        m_file_handler_map[file_path].erase( handler );
    }
}
