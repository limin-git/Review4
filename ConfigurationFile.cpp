#include "stdafx.h"
#include "ConfigurationFile.h"
#include "ICommandLine.h"
#include "IFileChangeManager.h"
#include "FileUtility.h"


ConfigurationFile::ConfigurationFile()
{
    boost::program_options::variables_map& vm = ICommandLine::instance().get_variables_map();

    if ( vm.count( "config-file" ) )
    {
        std::vector<std::wstring> config_files = vm[ "config-file" ].as< std::vector<std::wstring> >();

        BOOST_FOREACH( const std::wstring& config_file, config_files )
        {
            boost::filesystem::path p = boost::filesystem::system_complete( config_file );
            load_config_file( p );
            IFileChangeManager::instance().add_handler( p, this );
        }
    }
}


ConfigurationFile::~ConfigurationFile()
{
    BOOST_FOREACH( const ConfigurationFileMap::value_type& config_file, m_config_file_map )
    {
        IFileChangeManager::instance().remove_handler( config_file.first, this );
    }
}


IConfigurationFile& ConfigurationFile::add_options_description( boost::program_options::options_description& desc )
{
    m_descriptions.add( desc );

    BOOST_FOREACH( const ConfigurationFileMap::value_type& config_file, m_config_file_map )
    {
        parse_config_file( config_file.first );
    }

    return *this;
}


IConfigurationFile& ConfigurationFile::add_observer( IConfigurationFileObserver* observer )
{
    m_observers.insert( observer );

    BOOST_FOREACH( const FileVariablesMap::value_type& vm, m_variables_map )
    {
        static const boost::program_options::variables_map empty;
        observer->options_changed( vm.second, empty );
    }

    return *this;
}


void ConfigurationFile::remove_observer( IConfigurationFileObserver* observer )
{
    m_observers.erase( observer );
}


po::options_description& ConfigurationFile::options_description()
{
    return m_descriptions;
}


po::variables_map& ConfigurationFile::variables_map()
{
    return m_all_variables_map;
}


void ConfigurationFile::last_write_time_changed( const boost::filesystem::path& config_file )
{
    if ( load_config_file( config_file ) )
    {
        parse_config_file( config_file );

        BOOST_FOREACH( IConfigurationFileObserver* observer, m_observers )
        {
            BOOST_FOREACH( const ConfigurationFileMap::value_type& config, m_config_file_map )
            {
                boost::thread t( boost::bind( &ConfigurationFile::notify_observer_thread, this, observer, config.first ) );
            }
        }
    }
}


bool ConfigurationFile::load_config_file( const boost::filesystem::path& config_file )
{
    std::wstring ws = Utility::wstring_from_file( config_file.c_str() );

    if ( ws == m_config_file_map[config_file] )
    {
        return false;
    }

    m_config_file_map[config_file].swap( ws );
    return true;
}


void ConfigurationFile::parse_config_file( const boost::filesystem::path& config_file )
{
    std::wstringstream strm( m_config_file_map[config_file] );
    m_variables_map[config_file].swap( m_variables_map_old[config_file] );
    boost::program_options::variables_map& vm = m_variables_map[config_file];

    try
    {
        vm.clear();
        store( boost::program_options::parse_config_file<wchar_t>( strm, m_descriptions, true ), vm );
        notify( vm );
        merge_variables_map( m_all_variables_map, vm );
    }
    catch ( std::exception& e )
    {
        std::cout << "error: " << e.what() << std::endl;
    }
}


void ConfigurationFile::notify_observer_thread( IConfigurationFileObserver* observer, boost::filesystem::path& config_file )
{
    observer->options_changed( m_variables_map[config_file], m_variables_map_old[config_file] );
}


void ConfigurationFile::merge_variables_map( po::variables_map& vm, const po::variables_map& rhs )
{
    BOOST_FOREACH( const po::variables_map::value_type& v, rhs )
    {
        vm.erase( v.first );
        vm.insert( v );
    }
}
