#pragma once


struct IWallpaper
{
    virtual ~IWallpaper() {}
    virtual void run() = 0;
    static IWallpaper& instance();
    static void remove();
};
