#pragma once
#include "IConfigurationFileObserver.h"


struct IConfigurationFile
{
    virtual ~IConfigurationFile() {}
    virtual IConfigurationFile& add_options_description( po::options_description& options ) = 0;
    virtual IConfigurationFile& add_observer( IConfigurationFileObserver* observer ) = 0;
    virtual void remove_observer( IConfigurationFileObserver* observer ) = 0;
    virtual po::options_description& options_description() = 0;
    virtual po::variables_map& variables_map() = 0;
    static IConfigurationFile& instance();
    static void remove();
};
