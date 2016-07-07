#include "stdafx.h"
#include "IInputSender.h"
#include "InputSender.h"


static IInputSender* s_input_sender = NULL;


IInputSender& IInputSender::instance()
{
    if ( s_input_sender == NULL )
    {
        s_input_sender = new InputSender;
    }

    return *s_input_sender;
}


void IInputSender::remove()
{
    delete s_input_sender;
    s_input_sender = NULL;
}
