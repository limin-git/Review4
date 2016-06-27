#include "stdafx.h"
#include "FileSystem.h"


namespace FileSystem
{

    bool is_parent_path( const boost::filesystem::path& parent, const boost::filesystem::path& child )
    {
        for ( boost::filesystem::path p = child.parent_path(); child.has_parent_path(); p = p.parent_path() )
        {
            if ( p == parent )
            {
                return true;
            }
        }

        return false;
    }

}
