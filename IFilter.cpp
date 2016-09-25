#include "stdafx.h"
#include "IFilter.h"
#include "Filter.h"


IFilter& IFilter::instance()
{
    static Filter filter;
    return filter;
}
