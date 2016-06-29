#pragma once
#include "IConfigurationFile.h"


struct ConfigurationFile : public IConfigurationFile
{
    virtual ~ConfigurationFile() {}
    virtual void add_options_description( boost::program_options::options_description& desc );
    virtual void add_observer( IConfigurationFileObserver* observer );
    virtual void remove_observer( IConfigurationFileObserver* observer );
};
