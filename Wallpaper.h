#pragma once
#include "IWallpaper.h"
#include "IInputHandler.h"

namespace fs = boost::filesystem;


struct Wallpaper : IWallpaper,
                   IInputHandler
{
public:

    Wallpaper();
    ~Wallpaper();
    virtual void run();

protected:

    void remove_current_picture();

private:

    void search_pictures();
    void set_wallpaper();

private:

    fs::path m_directory;
    fs::path m_recycle_directory;
    std::list<fs::path> m_pictures;
    std::list<fs::path>::iterator m_current;
};
