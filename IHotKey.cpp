#include "stdafx.h"
#include "IHotKey.h"
#include "HotKey.h"


IHotKey& IHotKey::instance()
{
    static boost::mutex mutex;
    boost::lock_guard<boost::mutex> lock( mutex );
    static HotKey hotkey;
    return hotkey;
}
