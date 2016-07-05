#pragma once
#include "IWallpaper.h"

namespace fs = boost::filesystem;


struct Wallpaper : IWallpaper
{
public:

    Wallpaper();

private:

    fs::path m_directory;
    std::list<fs::path> m_pictures;
};
