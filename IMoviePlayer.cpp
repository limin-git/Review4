#include "stdafx.h"
#include "IMoviePlayer.h"
#include "MpcPlayer.h"


IMoviePlayer& IMoviePlayer::instance()
{
    static MpcPlayer player;
    return player;
}
