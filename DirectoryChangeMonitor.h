#pragma once
#include "IDirectoryChangeMonitor.h"
#include "DirectoryChangeMonitorThread.h"


class DirectoryChangeMonitor : public IDirectoryChangeMonitor
{
public:

    virtual void add_file_change_handler( const boost::filesystem::path& file_path, IDirectoryChangeHandler* handler );
    virtual void remove_file_change_handler( const boost::filesystem::path& file_path, IDirectoryChangeHandler* handler );

private:

    typedef std::map<boost::thread*, DirectoryChangeMonitorThread> DirectoryChangeMonitorThreadMap;
    DirectoryChangeMonitorThreadMap m_threads;
};
