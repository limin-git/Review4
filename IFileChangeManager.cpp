#include "stdafx.h"
#include "IFileChangeManager.h"
#include "FileChangeManager.h"


static IFileChangeManager* s_file_change_manager = NULL;


IFileChangeManager& IFileChangeManager::instance()
{
    if ( NULL == s_file_change_manager )
    {
        s_file_change_manager = new FileChangeManager;
    }

    return *s_file_change_manager;
}


void remove()
{
    delete s_file_change_manager;
    s_file_change_manager = NULL;
}
