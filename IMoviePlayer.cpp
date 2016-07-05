#include "stdafx.h"
#include "IMoviePlayer.h"
#include "MpcPlayer.h"


static IMoviePlayer* g_movie_player = NULL;


IMoviePlayer& IMoviePlayer::instance()
{
    if ( g_movie_player == NULL )
    {
        g_movie_player = new MpcPlayer;
    }

    return *g_movie_player;
}


void IMoviePlayer::remove()
{
    delete g_movie_player;
    g_movie_player = NULL;
}
