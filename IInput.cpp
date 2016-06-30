#include "stdafx.h"
#include "IInput.h"
#include "Input.h"


IInput* g_input = NULL;


IInput& IInput::instance()
{
    if ( g_input == NULL )
    {
        g_input = new Input;
    }

    return *g_input;
}


void IInput::remove()
{
    if ( g_input != NULL )
    {
        delete g_input;
        g_input = NULL;
    }
}
