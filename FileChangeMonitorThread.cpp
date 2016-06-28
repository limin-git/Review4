#include "stdafx.h"
#include "FileChangeMonitorThread.h"
#include "FileSystem.h"
#include "WriteConsoleHelper.h"


FileChangeMonitorThread::FileChangeMonitorThread()
{
    m_running = true;
    m_handle = NULL;
    m_thread = NULL;
}


FileChangeMonitorThread::~FileChangeMonitorThread()
{
    terminate();
}


void FileChangeMonitorThread::initialize( const fs::path& file_path, IFileChangeHandler* handler )
{
    m_dir = file_path.parent_path();
    m_file_handler_map[file_path].insert( handler );
    m_file_time_map[file_path] = boost::filesystem::last_write_time( file_path );
}


void FileChangeMonitorThread::start()
{
    m_thread = new boost::thread( boost::ref(*this ) );
}


void FileChangeMonitorThread::terminate()
{
    if ( m_running )
    {
        m_running = false;
    }

    if ( m_handle != NULL && m_thread != NULL )
    {
        FindCloseChangeNotification( m_handle );
        m_thread->join();
        delete m_thread;
        m_thread = NULL;
        m_handle = NULL;
    }
}


void FileChangeMonitorThread::operator()()
{
    m_handle = ::FindFirstChangeNotification( m_dir.wstring().c_str(), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE );

    if ( INVALID_HANDLE_VALUE == m_handle )
    {
        return;
    }

    stdcout << "start monitoring " << m_dir.wstring() << "\n";

    while ( m_running )
    {
        DWORD status = ::WaitForSingleObject( m_handle, INFINITE );

        if ( !m_running )
        {
            break;
        }

        if ( WAIT_OBJECT_0 == status )
        {
            notify_handlers();

            if ( ::FindNextChangeNotification( m_handle ) == FALSE )
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    stdcout << "stop monitoring " << m_dir.wstring() << "\n";
}


void FileChangeMonitorThread::notify_handlers()
{
    BOOST_FOREACH( FileHandlerMap::value_type& v, m_file_handler_map )
    {
        const fs::path& file = v.first;
        FileChangeHandlerList& handlers = v.second;

        std::time_t t = boost::filesystem::last_write_time( file );

        if ( t != m_file_time_map[file] )
        {
            BOOST_FOREACH( IFileChangeHandler* handler, handlers )
            {
                handler->last_write_time_changed( file );
            }

            m_file_time_map[file] = t;
        }
    }
}


bool FileChangeMonitorThread::merge_child( const FileChangeMonitorThread& rhs )
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


bool FileChangeMonitorThread::is_monitoring( const fs::path& file_path )
{
    return ( m_file_time_map.find( file_path ) != m_file_time_map.end() );
}


void FileChangeMonitorThread::remove_handler( const fs::path& file_path, IFileChangeHandler* handler )
{
    FileHandlerMap::iterator it = m_file_handler_map.find( file_path );

    if ( it != m_file_handler_map.end() )
    {
        it->second.erase( handler );

        if ( it->second.empty() )
        {
            m_file_handler_map.erase( it );
            m_file_time_map.erase( file_path );
        }
    }
}
