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


    std::vector<fs::path> get_files_of_directory( const fs::path& dir )
    {
        std::vector<fs::path> files;
        get_files_of_directory_if_impl( files, dir );
        return files;
    }


    std::vector<fs::path> get_files_of_directory_n( const fs::path& dir, size_t first_n )
    {
        std::vector<fs::path> files;
        get_files_of_directory_if_impl( files, dir, Pred(), first_n );
        return files;
    }


    std::vector<fs::path> get_files_of_directory_if( const fs::path& dir, const Pred& pred )
    {
        std::vector<fs::path> files;
        get_files_of_directory_if_impl( files, dir, pred );
        return files;
    }


    std::vector<fs::path> get_files_of_directory_if_n( const fs::path& dir, const Pred& pred, size_t first_n )
    {
        std::vector<fs::path> files;
        get_files_of_directory_if_impl( files, dir, pred, first_n );
        return files;
    }


    std::list<fs::path> get_file_list_of_directory( const fs::path& dir )
    {
        std::list<fs::path> files;
        get_files_of_directory_if_impl( files, dir );
        return files;
    }


    std::list<fs::path> get_file_list_of_directory_n( const fs::path& dir, size_t first_n )
    {
        std::list<fs::path> files;
        get_files_of_directory_if_impl( files, dir, Pred(), first_n );
        return files;
    }


    std::list<fs::path> get_file_list_of_directory_if( const fs::path& dir, const Pred& pred )
    {
        std::list<fs::path> files;
        get_files_of_directory_if_impl( files, dir, pred );
        return files;
    }


    std::list<fs::path> get_file_list_of_directory_if_n( const fs::path& dir, const Pred& pred, size_t first_n )
    {
        std::list<fs::path> files;
        get_files_of_directory_if_impl( files, dir, pred, first_n );
        return files;
    }

}
