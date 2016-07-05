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


IConfigurationFile& ConfigurationFile::add_options_description( boost::program_options::options_description& options_description )
{
    bool parse = false;

    BOOST_FOREACH( boost::shared_ptr<po::option_description> option, options_description.options() )
    {
        if ( NULL == m_options_description.find_nothrow( option->long_name(), false ) )
        {
            m_options_description.add( option );
            parse = true;
        }
    }

    if ( parse )
    {
        merge_variables_map( m_variables_map, parse_config_files( options_description ) );
    }

    return *this;
}


IConfigurationFile& ConfigurationFile::add_observer( IConfigurationFileObserver* observer )
{
    m_observers.insert( observer );
    static const boost::program_options::variables_map empty;
    observer->options_changed( m_variables_map, empty );
    return *this;
}


IConfigurationFile& ConfigurationFile::remove_observer( IConfigurationFileObserver* observer )
{
    m_observers.erase( observer );
    return *this;
}


po::options_description& ConfigurationFile::options_description()
{
    return m_options_description;
}


po::variables_map& ConfigurationFile::variables_map()
{
    return m_variables_map;
}


void ConfigurationFile::last_write_time_changed( const boost::filesystem::path& config_file )
{
    if ( load_config_file( config_file ) )
    {
        m_variables_map_old = m_variables_map;
        merge_variables_map( m_variables_map, parse_config_file( m_options_description, config_file ) );

        BOOST_FOREACH( IConfigurationFileObserver* observer, m_observers )
        {
            boost::thread t( boost::bind( &ConfigurationFile::notify_observer_thread, this, observer ) );
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


void ConfigurationFile::notify_observer_thread( IConfigurationFileObserver* observer )
{
    observer->options_changed( m_variables_map, m_variables_map_old );
}


void ConfigurationFile::merge_variables_map( po::variables_map& dst, const po::variables_map& src )
{
    BOOST_FOREACH( const po::variables_map::value_type& v, src )
    {
        dst.erase( v.first );
        dst.insert( v );
    }
}


po::variables_map ConfigurationFile::parse_config_file( const po::options_description& options, const fs::path& config_file )
{
    po::variables_map vm;
    std::wstringstream strm( m_config_file_map[config_file] );
    store( po::parse_config_file<wchar_t>( strm, options, true ), vm );
    return vm;
}


po::variables_map ConfigurationFile::parse_config_files( const po::options_description& options )
{
    if ( 1 == m_config_file_map.size() )
    {
        return parse_config_file( options, m_config_file_map.begin()->first );
    }

    po::variables_map all;

    BOOST_FOREACH( const ConfigurationFileMap::value_type& config_file, m_config_file_map )
    {
        po::variables_map vm;
        std::wstringstream strm( config_file.second );
        store( po::parse_config_file<wchar_t>( strm, options, true ), vm );
        merge_variables_map( all, vm );
    }

    return all;
}
