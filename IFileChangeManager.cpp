#include "stdafx.h"
#include "IFileChangeManager.h"
#include "FileChangeManager.h"


static IFileChangeManager* g_file_change_manager = NULL;


IFileChangeManager& IFileChangeManager::instance()
{
    if ( NULL == g_file_change_manager )
    {
        g_file_change_manager = new FileChangeManager;
    }

    return *g_file_change_manager;
}


void remove()
{
    if ( g_file_change_manager != NULL )
    {
        delete g_file_change_manager;
        g_file_change_manager = NULL;
    }
}
