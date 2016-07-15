#pragma once
namespace fs = boost::filesystem;


namespace Utility
{
    bool is_picture( const fs::path& file_path );
    void set_system_wallpaper( const fs::path& picture );
}
