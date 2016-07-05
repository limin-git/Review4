#include "stdafx.h"
#include "ISound.h"
#include "Sound.h"


static ISound* g_sound = NULL;


ISound& ISound::instance()
{
    if ( g_sound == NULL )
    {
        g_sound = new Sound;
    }

    return *g_sound;
}


void ISound::remove()
{
    delete g_sound;
    g_sound = NULL;
}
