#include "stdafx.h"
#include "ILog.h"
#include "Log.h"


ILog& ILog::instance()
{
    static Log log;
    return log;
}
