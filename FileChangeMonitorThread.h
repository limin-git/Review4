#pragma once
#include "IFileChangeManager.h "

namespace fs = boost::filesystem;
typedef std::set<IFileChangeHandler*> FileChangeHandlerList;
typedef std::map<fs::path, std::time_t> FileTimeMap;
typedef std::map<fs::path, FileChangeHandlerList> FileHandlerMap;


class FileChangeMonitorThread
{
public:

    FileChangeMonitorThread();
    ~FileChangeMonitorThread();
    void initialize( const fs::path& file_path, IFileChangeHandler* handler );
    void start();
    void terminate();
    void operator()();
    void notify_handlers();
    bool merge_child( const FileChangeMonitorThread& rhs );
    bool is_monitoring( const fs::path& file_path );
    void remove_handler( const fs::path& file_path, IFileChangeHandler* handler );
    bool empty() { return m_file_handler_map.empty(); }

private:

    fs::path m_dir;
    FileHandlerMap m_file_handler_map;
    FileTimeMap m_file_time_map;
    HANDLE m_handle;
    bool m_running;
    boost::thread* m_thread;
};
