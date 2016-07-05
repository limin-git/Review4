#include "stdafx.h"
#include "Utility.h"


namespace Utility
{

    size_t random( size_t lo, size_t hi )
    {
        boost::random::mt19937 gen( static_cast<boost::uint32_t>( std::time(0) ) );
        boost::random::uniform_int_distribution<> dist( lo, hi );
        return dist( gen );
    }


    void set_system_wallpaper( const fs::path& picture )
    {
        SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, const_cast<wchar_t*>( picture.wstring().c_str() ), SPIF_UPDATEINIFILE);
    }


    std::vector<fs::path> get_files_of_directory( const fs::path& dir )
    {
        std::vector<fs::path> files;

        if ( !exists( dir ) )
        {
            return files;
        }

        fs::recursive_directory_iterator end;
        for ( fs::recursive_directory_iterator it( dir ); it != end; ++it )
        {
            if ( false != is_directory( it->status() ) )
            {
                files.push_back( it->path() );
            }
        }

        return files;
    }


    bool is_picture( const fs::path& file_path )
    {
        std::ifstream is( file_path.wstring().c_str(), std::ios::in | std::ios::binary );

        if ( !is )
        {
            return false;
        }

        unsigned char ch[2] = { 0 };

        is.unsetf( std::ios::skipws );
        is.read( (char*)ch, 2 );

        if ( ch[0] == 0xFF && ch[1] == 0xD8 ) // JPEG
        {
            return true;
        }
        else if ( ch[0] == 0x4D && ch[1] == 0x42 ) // BMP
        {
            return true;
        }
        else
        {
            return false;
        }
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

}
