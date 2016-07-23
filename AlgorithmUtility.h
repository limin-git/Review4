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

    template<typename T>
    void random_shuffle( std::list<T>& list )
    {
        std::vector<T> vs( list.begin(), list.end() );
        std::srand( static_cast<unsigned int>( std::time( NULL ) ) );
        std::random_shuffle( vs.begin(), vs.end() );
        list.assign( vs.begin(), vs.end() );
    }

    template<typename T>
    void random_shuffle( std::vector<T>& list )
    {
        std::srand( static_cast<unsigned int>( std::time( NULL ) ) );
        std::random_shuffle( list.begin(), list.end() );
    }
}
