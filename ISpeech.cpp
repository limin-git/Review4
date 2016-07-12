#include "stdafx.h"
#include "ISpeech.h"
#include "Speech.h"


ISpeech& ISpeech::instance()
{
    static Speech speech;
    return speech;
}
