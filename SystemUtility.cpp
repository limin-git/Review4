#include "stdafx.h"
#include "SystemUtility.h"


namespace Utility
{

    void set_system_wallpaper( const fs::path& picture )
    {
        if ( is_picture( picture ) )
        {
            SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, const_cast<wchar_t*>( picture.wstring().c_str() ), SPIF_UPDATEINIFILE);
        }
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

        return false;
    }

}
