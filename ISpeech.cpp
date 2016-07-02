#include "stdafx.h"
#include "ISpeech.h"
#include "Speech.h"


static ISpeech* g_speech = NULL;


ISpeech& ISpeech::instance()
{
    if ( g_speech == NULL )
    {
        g_speech = new Speech;
    }

    return *g_speech;
}


void ISpeech::remove()
{
    if ( g_speech != NULL )
    {
        delete g_speech;
        g_speech = NULL;
    }
}
