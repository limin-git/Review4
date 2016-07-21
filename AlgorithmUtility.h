#pragma once


namespace Utility
{
    size_t random( int lo, int hi );

    template<typename String, typename RangeT>
    void remove_if_is_any_of( String& str, RangeT set )
    {
        str.erase( std::remove_if( str.begin(), str.end(), boost::is_any_of( set ) ), str.end() );
    }

    template<typename String, typename RangeT>
    String remove_if_is_any_of_copy( String& str, RangeT set )
    {
        String s = str;
        s.erase( std::remove_if( s.begin(), s.end(), boost::is_any_of( set ) ), s.end() );
        return s;
    }
}
