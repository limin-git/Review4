#include "stdafx.h"
#include "IInput.h"
#include "Input.h"


static IInput* s_input = NULL;


IInput& IInput::instance()
{
    if ( s_input == NULL )
    {
        s_input = new Input;
    }

    return *s_input;
}


void IInput::remove()
{
    delete s_input;
    s_input = NULL;
}
