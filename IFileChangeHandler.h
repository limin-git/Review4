#pragma once


struct IFileChangeHandler
{
    virtual ~IFileChangeHandler() {};
    virtual void last_write_time_changed( const boost::filesystem::path& file ) = 0;
};
