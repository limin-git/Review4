#include "stdafx.h"
#include "IWallpaper.h"
#include "Wallpaper.h"


static IWallpaper* s_wallpaper = NULL;


IWallpaper& IWallpaper::instance()
{
    if ( s_wallpaper == NULL )
    {
        s_wallpaper = new Wallpaper;
    }

    return *s_wallpaper;
}


void IWallpaper::remove()
{
    delete s_wallpaper;
    s_wallpaper = NULL;
}
