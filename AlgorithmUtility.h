#pragma once


namespace Utility
{
    template<typename String, typename RangeT>
    void remove_if_is_any_of( String& str, RangeT set )
    {
        str.erase( std::remove_if( str.begin(), str.end(), boost::is_any_of( set ) ), str.end() );
    }
}
