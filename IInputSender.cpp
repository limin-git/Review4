#include "stdafx.h"
#include "IInputSender.h"
#include "InputSender.h"


IInputSender& IInputSender::instance()
{
    static InputSender input_sender;
    return input_sender;
}
