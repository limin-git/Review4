#include "stdafx.h"
#include "Wallpaper.h"
#include "IConfigurationFile.h"

namespace po = boost::program_options;


Wallpaper::Wallpaper()
{
    po::options_description options;
    options.add_options()
        ( "wallpaper.path", po::wvalue<std::wstring>(), "history directory path" )
        ;
    m_directory =
        IConfigurationFile::instance()
        .add_options_description( options )
        .variables_map()["wallpaper.path"].as<std::wstring>();
    system_complete( m_directory );
};
