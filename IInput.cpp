#include "stdafx.h"
#include "IInput.h"
#include "Input.h"


IInput& IInput::instance()
{
    static boost::mutex mutex;
    boost::lock_guard<boost::mutex> lock( mutex );
    static Input input;
    return input;
}
