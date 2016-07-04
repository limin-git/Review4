#pragma once
#include "ISchedule.h"


struct Schedule : ISchedule
{
public:
    Schedule();
    ~Schedule();
    virtual ISlideshowPtr next_slideshow();

private:

    bool is_expired( size_t key, std::time_t current = std::time( NULL ) );
    void caclute_schedule_time();

private:
    
    std::vector<std::time_t> m_schedule;
    std::set<size_t> m_expired;
    std::map<std::time_t, size_t> m_schedule_map;
};
