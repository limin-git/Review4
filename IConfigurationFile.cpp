#include "stdafx.h"
#include "IConfigurationFile.h"
#include "ConfigurationFile.h"


static IConfigurationFile* s_configuration_file = NULL;


IConfigurationFile::IConfigurationFile& instance()
{
    if ( s_configuration_file == NULL )
    {
        s_configuration_file = new ConfigurationFile;
    }
}


void IConfigurationFile::remove()
{

}
