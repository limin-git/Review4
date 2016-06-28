#pragma once
#include "IFileChangeManager.h"
#include "FileChangeMonitorThread.h"


typedef std::list<FileChangeMonitorThread> FileChangeMonitorThreadList;


class FileChangeManager : public IFileChangeManager
{
public:

    virtual void add_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler );
    virtual void remove_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler );

private:

    FileChangeMonitorThreadList m_threads;
};
