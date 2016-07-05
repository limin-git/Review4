#pragma once
#include "TimeUtility.h"
#include "StringUtility.h"
#include "UnicodeUtility.h"
#include "SoundUtility.h"
#include "ConsoleUtility.h"
#include "FileUtility.h"
#include "WriteConsoleHelper.h"

#ifdef max
#undef max
#undef min
#endif

namespace fs = boost::filesystem;


namespace Utility
{
    size_t random( size_t lo, size_t hi );
    void set_system_wallpaper( const fs::path& picture );
    std::vector<fs::path> get_files_of_directory( const fs::path& dir );
    bool is_picture( const fs::path& file_path );
    bool remove_file( const fs::path& file_path );
    bool rename_file( const fs::path& old_path, const fs::path& new_path );

    template<typename T>
    bool updated( const char* name, const boost::program_options::variables_map& vm, const boost::program_options::variables_map& old )
    {
        if ( vm.count( name ) )
        {
            if ( ( !old.count( name ) ) || ( vm[name].as<T>() != old[name].as<T>() ) )
            {
                return true;
            }
        }

        return false;
    }
}
