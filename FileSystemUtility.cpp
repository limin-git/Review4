#include "stdafx.h"
#include "FileSystemUtility.h"


namespace Utility
{

    bool is_parent_path( const fs::path& parent, const fs::path& child )
    {
        fs::path cp = child;

        while ( cp.has_parent_path() )
        {
            cp = cp.parent_path();

            if ( cp == parent )
            {
                return true;
            }
        }

        return false;
    }


    bool remove_file( const fs::path& file_path )
    {
        fs::path p( file_path );

        if ( exists( file_path ) )
        {
            boost::system::error_code ec;
            permissions( file_path, fs::all_all );
            remove( file_path, ec );

            if ( ec )
            {
                return false;
            }
        }

        return true;
    }


    bool rename_file( const fs::path& old_path, const fs::path& new_path )
    {
        if ( exists( old_path ) && ( !exists( new_path ) ) )
        {
            boost::system::error_code ec;
            permissions( old_path, fs::all_all );
            rename( old_path, new_path, ec );

            if ( ec )
            {
                return false;
            }
        }

        return true;
    }


    std::list<fs::path> get_files_of_directory( const fs::path& dir, size_t first_n )
    {
        std::list<fs::path> files;
        size_t n = 0;

        if ( ! exists( dir ) || ! is_directory( dir ) )
        {
            return files;
        }

        fs::recursive_directory_iterator end;
        for ( fs::recursive_directory_iterator it( dir ); it != end; ++it )
        {
            if ( ! is_directory( it->status() ) )
            {
                files.push_back( it->path() );

                if ( first_n <= ++n )
                {
                    break;
                }
            }
        }

        return files;
    }


    std::list<fs::path> get_files_of_directory_if( const fs::path& dir, const boost::function<bool(const fs::path&)>& predicate, size_t first_n )
    {
        std::list<fs::path> files;
        size_t n = 0;

        if ( ! exists( dir ) || ! is_directory( dir ) )
        {
            return files;
        }

        fs::recursive_directory_iterator end;
        for ( fs::recursive_directory_iterator it( dir ); it != end; ++it )
        {
            if ( ! is_directory( it->status() ) )
            {
                if ( predicate( it->path() ) )
                {
                    files.push_back( it->path() );

                    if ( first_n <= ++n )
                    {
                        break;
                    }
                }
            }
        }

        return files;
    }

}
