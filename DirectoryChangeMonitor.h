#pragma once
#include "IDirectoryChangeMonitor.h"


namespace fs = boost::filesystem;


class DirectoryChangeMonitor : public IDirectoryChangeMonitor
{
public:

    typedef boost::signals2::signal<void()> SignalType;

    virtual void add_file_change_handler( const fs::path& file_path, FileChangeHandler handler );
    virtual void remove_file_change_handler( const fs::path& file_path, FileChangeHandler handler );

private:

    typedef std::map<fs::path, boost::thread*> DirectoryThreadMap;               // 1:1
    typedef std::map< fs::path, std::vector<fs::path> > DirectoryFilesMap;  // 1:N
    typedef std::map<fs::path, SignalType*> FileSignalMap;                      // 1:1

    DirectoryThreadMap m_directory_thread_map;
    DirectoryFilesMap m_directory_files_map;
    FileSignalMap m_file_signal_map;
};
