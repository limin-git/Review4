#pragma once
namespace fs = boost::filesystem;


namespace Utility
{
    bool is_parent_path( const fs::path& parent, const fs::path& child );
    bool remove_file( const fs::path& file_path );
    bool rename_file( const fs::path& old_path, const fs::path& new_path );
    std::list<fs::path> get_files_of_directory( const fs::path& dir );
}
