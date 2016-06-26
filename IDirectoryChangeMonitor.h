#pragma once


typedef boost::function<void()> FileChangeHandler;


class IDirectoryChangeMonitor
{
public:

    virtual ~IDirectoryChangeMonitor() {};
    virtual void add_file_change_handler( const boost::filesystem::path& file_path, FileChangeHandler handler ) = 0;
    virtual void remove_file_change_handler( const boost::filesystem::path& file_path, FileChangeHandler handler ) = 0;

    static IDirectoryChangeMonitor* instance();
    static void remove();
};
