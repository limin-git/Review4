#include "stdafx.h"
#include "IGlobalSignals.h"
#include "GlobalSignals.h"


IGlobalSignals& IGlobalSignals::instance()
{
    static GlobalSignals global_signals;
    return global_signals;
}
