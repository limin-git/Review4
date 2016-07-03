#include "stdafx.h"
#include "IConfigurationFile.h"
#include "ConfigurationFile.h"


static IConfigurationFile* s_configuration_file = NULL;


IConfigurationFile& IConfigurationFile::instance()
{
    if ( s_configuration_file == NULL )
    {
        s_configuration_file = new ConfigurationFile;
    }

    return *s_configuration_file;
}


void IConfigurationFile::remove()
{
    delete s_configuration_file;
    s_configuration_file = NULL;
}
