#include "stdafx.h"
#include "GlobalSignals.h"


GlobalSignals::GlobalSignals()
    : m_next_slide( 0 )
{
}


GlobalSignals::~GlobalSignals()
{
}


void GlobalSignals::signal_next_slide()
{
    m_next_slide.post();
}


void GlobalSignals::wait_next_slide()
{
    m_next_slide.wait();
}
