#include "stdafx.h"
#include "AlgorithmUtility.h"


namespace Utility
{

    size_t random( int lo, int hi )
    {
        boost::random::mt19937 gen( static_cast<boost::uint32_t>( std::time(0) ) );
        boost::random::uniform_int_distribution<> dist( lo, hi );
        return dist( gen );
    }

}
