#pragma once


struct IDirectoryChangeHandler
{
    virtual ~IDirectoryChangeHandler() = 0;
    virtual void last_write_time_changed( const boost::filesystem::path& file ) = 0;
};
