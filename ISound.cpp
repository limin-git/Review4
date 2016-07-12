#include "stdafx.h"
#include "ISound.h"
#include "Sound.h"


ISound& ISound::instance()
{
    static Sound sound;
    return sound;
}
