#include "stdafx.h"
#include "IScheduler.h"
#include "Scheduler.h"


IScheduler& IScheduler::instance()
{
    static Scheduler scheduler;
    return scheduler;
}
