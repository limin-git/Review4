#include "stdafx.h"
#include "IHotKey.h"
#include "HotKey.h"


static IHotKey* s_hot_key = NULL;


IHotKey& IHotKey::instance()
{
    if ( s_hot_key == NULL )
    {
        s_hot_key = new HotKey;
    }

    return *s_hot_key;
}


void IHotKey::remove()
{
    delete s_hot_key;
    s_hot_key = NULL;
}
