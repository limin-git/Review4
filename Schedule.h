#pragma once
#include "ISchedule.h"


struct Schedule : ISchedule
{
public:
    Schedule();
    ~Schedule();
    virtual ISlideshowPtr next_slideshow();

private:
    
    std::vector<std::time_t> m_schedule;
};
