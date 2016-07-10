#pragma once
#include "IConfigurationFile.h"
#include "IFileChangeHandler.h"
#include "IFileChangeManager.h"
#include "ICommandLine.h"
#include "Singleton.h"

namespace fs = boost::filesystem;


class ConfigurationFile : public IConfigurationFile,
                          public IFileChangeHandler
{
public:

    ConfigurationFile();
    virtual ~ConfigurationFile();
    virtual IConfigurationFile& add_options_description( boost::program_options::options_description& options );
    virtual IConfigurationFile& add_observer( IConfigurationFileObserver* observer );
    virtual IConfigurationFile& remove_observer( IConfigurationFileObserver* observer );
    virtual po::options_description& options_description();
    virtual po::variables_map& variables_map();

protected:

    virtual void last_write_time_changed( const boost::filesystem::path& file );

private:

    bool load_config_file( const fs::path& config_file );
    po::variables_map parse_config_file( const po::options_description& options, const fs::path& config_file );
    po::variables_map parse_config_files( const po::options_description& options );
    void merge_variables_map( po::variables_map& dst, const po::variables_map& src );
    void notify_observer_thread( IConfigurationFileObserver* observer );

private:

    typedef std::map<fs::path, std::wstring> ConfigurationFileMap;
    ConfigurationFileMap m_config_file_map;
    std::set<IConfigurationFileObserver*> m_observers;
    po::options_description m_options_description;
    po::variables_map m_variables_map;
    po::variables_map m_variables_map_old;
    Singleton<IFileChangeManager> m_file_change_manager;
    Singleton<ICommandLine> m_command_line;
};
