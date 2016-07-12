#include "stdafx.h"
#include "IEnglishPlayer.h"
#include "EnglishPlayer.h"


IEnglishPlayer& IEnglishPlayer::instance()
{
    static EnglishPlayer player;
    return player;
}
