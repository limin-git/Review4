#include "stdafx.h"
#include "ISchedule.h"
#include "Schedule.h"


static ISchedule* s_schedule = NULL;


ISchedule& ISchedule::instance()
{
    if ( s_schedule == NULL )
    {
        s_schedule = new Schedule;
    }

    return *s_schedule;
}


void ISchedule::remove()
{
    delete s_schedule;
    s_schedule = NULL;
}
