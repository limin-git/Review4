#include "stdafx.h"
#include "IEnglishPlayer.h"
#include "EnglishPlayer.h"


static IEnglishPlayer* g_english_player = NULL;


IEnglishPlayer& IEnglishPlayer::instance()
{
    if ( g_english_player == NULL )
    {
        g_english_player = new EnglishPlayer;
    }

    return *g_english_player;
}


void IEnglishPlayer::remove()
{
    delete g_english_player;
    g_english_player = NULL;
}
