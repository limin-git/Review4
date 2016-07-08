#pragma once

#ifdef max
#undef max
#undef min
#endif

namespace fs = boost::filesystem;
namespace po = boost::program_options;


namespace Utility
{
    template<typename T>
    bool updated( const char* name, const po::variables_map& vm, const po::variables_map& old )
    {
        if ( vm.count( name ) )
        {
            T new_value = vm[name].as<T>();

            if ( old.count( name ) )
            {
                T old_value = old[name].as<T>();
                return ( new_value != old_value );
            }
            else
            {
                return true;
            }
        }

        return false;
    }
}
