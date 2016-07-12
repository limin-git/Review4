#include "stdafx.h"
#include "IInput.h"
#include "Input.h"


IInput& IInput::instance()
{
    static Input input;
    return input;
}
