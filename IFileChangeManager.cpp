#include "stdafx.h"
#include "IFileChangeManager.h"
#include "FileChangeManager.h"


IFileChangeManager& IFileChangeManager::instance()
{
    static FileChangeManager manager;
    return manager;
}
