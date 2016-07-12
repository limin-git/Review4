#include "stdafx.h"
#include "IConfigurationFile.h"
#include "ConfigurationFile.h"


IConfigurationFile& IConfigurationFile::instance()
{
    static ConfigurationFile configuration_file;
    return configuration_file;
}
