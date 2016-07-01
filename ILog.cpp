#include "stdafx.h"
#include "ILog.h"
#include "Log.h"


static Log* g_log_instance = NULL;


ILog& ILog::instance()
{
    if ( g_log_instance == NULL )
    {
        g_log_instance = new Log;
    }

    return *g_log_instance;
}


void ILog::remove()
{
    if ( g_log_instance != NULL )
    {
        delete g_log_instance;
        g_log_instance = NULL;
    }
}
