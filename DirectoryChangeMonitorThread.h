#pragma once
#include "IDirectoryChangeMonitor.h "

namespace fs = boost::filesystem;
typedef std::set<IDirectoryChangeHandler*> FileChangeHandlerList;
typedef std::map<fs::path, std::time_t> FileTimeMap;
typedef std::map<fs::path, FileChangeHandlerList> FileHandlerMap;


class DirectoryChangeMonitorThread
{
public:

    DirectoryChangeMonitorThread( const fs::path& file_path, IDirectoryChangeHandler* handler );
    ~DirectoryChangeMonitorThread();
    void terminate();
    void operator()();
    void notify_handlers();
    bool merge_child( const DirectoryChangeMonitorThread& rhs );
    bool is_monitored( const fs::path& file_path );
    void remove_handler( const fs::path& file_path, IDirectoryChangeHandler* handler );

    fs::path m_dir;
    FileHandlerMap m_file_handler_map;
    FileTimeMap m_file_time_map;
    HANDLE m_handle;
    bool m_running;
};
