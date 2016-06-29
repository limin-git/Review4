#pragma once
#include "IConfigurationFile.h"
#include "IFileChangeHandler.h"


class ConfigurationFile : public IConfigurationFile,
                          public IFileChangeHandler
{
public:

    ConfigurationFile();
    virtual ~ConfigurationFile();
    virtual void add_options_description( boost::program_options::options_description& desc );
    virtual void add_observer( IConfigurationFileObserver* observer );
    virtual void remove_observer( IConfigurationFileObserver* observer );

protected:

    virtual void last_write_time_changed( const boost::filesystem::path& file );

private:

    bool load_config_file( const boost::filesystem::path& config_file );
    void parse_config_file( const boost::filesystem::path& config_file );
    void notify_observer_thread( IConfigurationFileObserver* observer, boost::filesystem::path& p );

private:

    typedef std::map<boost::filesystem::path, std::wstring> ConfigurationFileMap;
    ConfigurationFileMap m_config_file_map;
    std::set<IConfigurationFileObserver*> m_observers;
    boost::program_options::options_description m_descriptions;
    typedef std::map<boost::filesystem::path, boost::program_options::variables_map> FileVariablesMap;
    FileVariablesMap m_variables_map;
    FileVariablesMap m_variables_map_old;
};
