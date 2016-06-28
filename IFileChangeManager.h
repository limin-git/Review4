#pragma once
#include "IFileChangeHandler.h"


struct IFileChangeManager
{
    virtual ~IFileChangeManager() {};
    virtual void add_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler ) = 0;
    virtual void remove_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler ) = 0;

    static IFileChangeManager& instance();
    static void remove();
};
