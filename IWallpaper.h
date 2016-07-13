#pragma once


struct IWallpaper
{
    virtual ~IWallpaper() {}
    virtual void run() = 0;
};
