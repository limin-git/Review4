#pragma once
#include "IConfigurationFile.h"
#include "IFileChangeHandler.h"

namespace fs = boost::filesystem;


class ConfigurationFile : public IConfigurationFile,
                          public IFileChangeHandler
{
public:

    ConfigurationFile();
    virtual ~ConfigurationFile();
    virtual IConfigurationFile& add_options_description( boost::program_options::options_description& desc );
    virtual IConfigurationFile& add_observer( IConfigurationFileObserver* observer );
    virtual void remove_observer( IConfigurationFileObserver* observer );
    virtual po::options_description& options_description();
    virtual po::variables_map& variables_map();

protected:

    virtual void last_write_time_changed( const boost::filesystem::path& file );

private:

    bool load_config_file( const fs::path& config_file );
    void parse_config_file( const fs::path& config_file );
    void notify_observer_thread( IConfigurationFileObserver* observer, boost::filesystem::path& p );
    void merge_variables_map( po::variables_map& vm, const po::variables_map& rhs );

private:

    typedef std::map<fs::path, std::wstring> ConfigurationFileMap;
    ConfigurationFileMap m_config_file_map;
    std::set<IConfigurationFileObserver*> m_observers;
    po::options_description m_descriptions;
    typedef std::map<fs::path, po::variables_map> FileVariablesMap;
    FileVariablesMap m_variables_map;
    FileVariablesMap m_variables_map_old;
    po::variables_map m_all_variables_map;
};
