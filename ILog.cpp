#include "stdafx.h"
#include "ILog.h"
#include "Log.h"


static ILog* g_log = NULL;


ILog& ILog::instance()
{
    if ( g_log == NULL )
    {
        g_log = new Log;
    }

    return *g_log;
}


void ILog::remove()
{
    if ( g_log != NULL )
    {
        delete g_log;
        g_log = NULL;
    }
}
