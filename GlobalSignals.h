#pragma once
#include "IGlobalSignals.h"


struct GlobalSignals : IGlobalSignals
{
public:

    GlobalSignals();
    ~GlobalSignals();
    virtual void signal_next_slide();
    virtual void wait_next_slide();

private:

    boost::interprocess::interprocess_semaphore m_next_slide;
};
