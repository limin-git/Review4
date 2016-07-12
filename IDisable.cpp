#include "stdafx.h"
#include "IDisable.h"
#include "Disable.h"


IDisable& IDisable::instance()
{
    static Disable disable;
    return disable;
}
