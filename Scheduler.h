#pragma once
#include "IScheduler.h"
#include "IDisableObserver.h"
#include "ITextObserver.h"


struct Scheduler : IScheduler,
                   IDisableObserver,
                   ITextObserver
{
public:
    Scheduler();
    ~Scheduler();
    virtual ISlideshowPtr get_slideshow();
    virtual void disabled( size_t key );
    virtual void text_changed( IText* text );

private:

    void initialize_schedule();
    void select_candidates_thread();
    void schedule_next_time( size_t key );
    std::time_t get_next_time( size_t key, const std::time_t current = std::time( NULL ) );
    bool is_finished( size_t key );
    void set_title();

private:

    std::vector<std::time_t> m_schedule;
    std::set<size_t> m_candidates;
    typedef std::multimap<std::time_t, size_t> NextTimeMap;
    std::multimap<std::time_t, size_t> m_next_time_map;
    boost::interprocess::interprocess_semaphore m_select_candidates_semaphore;
    bool m_running;
    boost::thread* m_select_candidates_thread;
    bool m_once_per_session;
};
