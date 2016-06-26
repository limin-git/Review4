#include "stdafx.h"
#include "DirectoryChangeMonitor.h"
//#include "Log.h"


boost::thread make_thread();

void DirectoryChangeMonitor::add_file_change_handler( const fs::path& file_path, FileChangeHandler handler )
{
    if ( !fs::is_directory( file_path ) )
    {
        return;
    }

    fs::path p = fs::system_complete( file_path );

    for ( fs::path parent = p.parent_path(); parent.has_parent_path(); parent = parent.parent_path() )
    {
        DirectoryFilesMap::iterator it = m_directory_files_map.find( parent );

        if ( it != m_directory_files_map.end() )
        {
            std::vector<fs::path>& list = it->second;

            if ( std::find( list.begin(), list.end(), p ) != list.end() )
            {
                list.push_back( p );
            }

            SignalType*& signal = m_file_signal_map[p];

            if ( signal == NULL )
            {
                signal = new SignalType;
            }

            signal->connect( handler );
            return;
        }
    }
    
    fs::path pp = p.parent_path();
    m_directory_thread_map[pp] = new boost::thread; // TODO
    m_directory_files_map[pp].push_back( p );
    m_file_signal_map[p] = new SignalType;
    m_file_signal_map[p]->connect( handler );


    for ( DirectoryThreadMap::iterator it = m_directory_thread_map.begin(); it != m_directory_thread_map.end(); ++it )
    {
        const fs::path& dir = it->first;
        
        for ( fs::path parent = dir.parent_path(); parent.has_parent_path(); parent = parent.parent_path() )
        {
            if ( parent == pp )
            {
                //TODO
            }
        }
    }    
}


void DirectoryChangeMonitor::remove_file_change_handler( const fs::path& file_path, FileChangeHandler handler )
{
    fs::path p = fs::system_complete( file_path );

    FileSignalMap::iterator it = m_file_signal_map.find( p );

    if ( it != m_file_signal_map.end() )
    {
        const fs::path& pp = it->first;
        SignalType* signal = it->second;
        //signal->disconnect( handler );
    }
}



#if 0

void DirectoryChangeMonitor::watch_directory_thread( const std::wstring& file )
{
    if ( ! boost::filesystem::exists( file ) )
    {
        LOG << "can not find: " << file;
        return;
    }

    if ( ! boost::filesystem::is_regular_file( file ) )
    {
        LOG << "it is not a file: " << file;
        return;
    }

    std::wstring directory = boost::filesystem::system_complete( file ).parent_path().wstring();
    HANDLE handle = ::FindFirstChangeNotification( directory.c_str(), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE );

    if ( handle == INVALID_HANDLE_VALUE )
    {
        LOG << file << ", FindFirstChangeNotification error: " << ::GetLastError();
        return;
    }

    std::time_t last_write_time = boost::filesystem::last_write_time( file );

    while ( true )
    {
        DWORD status = ::WaitForSingleObject( handle, INFINITE );

        if ( WAIT_OBJECT_0 == status )
        {
            std::time_t t = boost::filesystem::last_write_time( file );

            if ( t != last_write_time )
            {
                last_write_time = t;
                LOG_DEBUG << file;
                (*m_signals[file])();
            }

            if ( ::FindNextChangeNotification( handle ) == FALSE )
            {
                LOG << file << ", FindNextChangeNotification error: " << ::GetLastError();
                return;
            }
        }
        else
        {
            LOG << file << ", WaitForSingleObject error: " << ::GetLastError();
            return;
        }
    }
}

#endif
