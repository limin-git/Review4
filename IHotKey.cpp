#include "stdafx.h"
#include "IHotKey.h"
#include "HotKey.h"


IHotKey& IHotKey::instance()
{
    static HotKey hotkey;
    return hotkey;
}
