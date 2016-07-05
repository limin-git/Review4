#include "stdafx.h"
#include "IDisable.h"
#include "Disable.h"


static IDisable* g_disable = NULL;


IDisable& IDisable::instance()
{
    if ( g_disable == NULL )
    {
        g_disable = new Disable;
    }

    return *g_disable;
}


void IDisable::remove()
{
    delete g_disable;
    g_disable = NULL;
}
