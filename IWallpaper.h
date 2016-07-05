#pragma once


struct IWallpaper
{
    virtual ~IWallpaper() {}
    static IWallpaper& instance();
    static void remove();
};
