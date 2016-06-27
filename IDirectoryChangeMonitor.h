#pragma once


struct IDirectoryChangeHandler;


struct IDirectoryChangeMonitor
{
    virtual ~IDirectoryChangeMonitor() = 0;
    virtual void add_file_change_handler( const boost::filesystem::path& file_path, IDirectoryChangeHandler* handler ) = 0;
    virtual void remove_file_change_handler( const boost::filesystem::path& file_path, IDirectoryChangeHandler* handler ) = 0;

    static IDirectoryChangeMonitor* instance();
    static void remove();
};
