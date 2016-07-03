#include "stdafx.h"
#include "IHistory.h"
#include "History.h"


static IHistory* s_history = NULL;


IHistory& IHistory::instance()
{
    if ( s_history == NULL )
    {
        s_history = new History;
    }

    return *s_history;
}


void IHistory::remove()
{
    delete s_history;
    s_history = NULL;
}
