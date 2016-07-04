#include "stdafx.h"
#include "IScheduler.h"
#include "Scheduler.h"


static IScheduler* s_scheduler = NULL;


IScheduler& IScheduler::instance()
{
    if ( s_scheduler == NULL )
    {
        s_scheduler = new Scheduler;
    }

    return *s_scheduler;
}


void IScheduler::remove()
{
    delete s_scheduler;
    s_scheduler = NULL;
}
