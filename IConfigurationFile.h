#pragma once
#include "IConfigurationFileObserver.h"


struct IConfigurationFile
{
    virtual ~IConfigurationFile() {}
    virtual IConfigurationFile& add_options_description( boost::program_options::options_description& desc ) = 0;
    virtual IConfigurationFile& add_observer( IConfigurationFileObserver* observer ) = 0;
    virtual void remove_observer( IConfigurationFileObserver* observer ) = 0;
    static IConfigurationFile& instance();
    static void remove();
};
