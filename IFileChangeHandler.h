#pragma once
namespace fs = boost::filesystem;


struct IFileChangeHandler
{
    virtual ~IFileChangeHandler() {};
    virtual void last_write_time_changed( const fs::path& file ) = 0;
};
