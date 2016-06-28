#include "stdafx.h"
#include "FileSystem.h"


namespace FileSystem
{

    bool is_parent_path( const boost::filesystem::path& parent, const boost::filesystem::path& child )
    {
        boost::filesystem::path cp = child;

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

}
