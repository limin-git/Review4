#include "stdafx.h"
#include "IHistory.h"
#include "History.h"


IHistory& IHistory::instance()
{
    static History history;
    return history;
}
