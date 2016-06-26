#include "stdafx.h"
#include "Log.h"
#include "ProgramOptions.h"
BOOST_LOG_ATTRIBUTE_KEYWORD(debug_level_attribute, "Level", std::wstring)


boost::log::sources::wlogger m_log;
boost::log::sources::wlogger m_log_error;
boost::log::sources::wlogger m_log_info;
boost::log::sources::wlogger m_log_debug;
boost::log::sources::wlogger m_log_trace;
boost::log::sources::wlogger m_log_test;


using namespace boost::log;
static boost::log::formatter m_formatter;
static boost::shared_ptr< sinks::synchronous_sink<sinks::text_ostream_backend> > m_console_sink;
static boost::shared_ptr< sinks::synchronous_sink<sinks::text_file_backend> > m_file_sink;
static std::vector<std::wstring> m_debug_levels;


boost::program_options::options_description Log::get_description()
{
    boost::program_options::options_description desc( "Log Options" );
    desc.add_options()
        ( "log.ini", boost::program_options::wvalue<std::wstring>(), "log configuration file" )
        ( "log.file", boost::program_options::wvalue<std::wstring>(), "log file name." )
        ( "log.rotation-size", boost::program_options::value<size_t>(), "log rotation size." )
        ( "log.levels", boost::program_options::wvalue< std::vector<std::wstring> >()->multitoken(), "error, info, debug, trace, *" )
        ( "log.no-console", boost::program_options::wvalue<std::wstring>(), "true or false" )
        ;

    return desc;
}


void Log::initialize( const boost::program_options::variables_map& vm )
{
    using namespace boost::log;

    m_log_error.add_attribute( "Level", boost::log::attributes::constant<std::wstring>( L"ERROR" ) );
    m_log_info.add_attribute( "Level", boost::log::attributes::constant<std::wstring>( L"INFO" ) );
    m_log_debug.add_attribute( "Level", boost::log::attributes::constant<std::wstring>( L"DEBUG" ) );
    m_log_trace.add_attribute( "Level", boost::log::attributes::constant<std::wstring>( L"TRACE" ) );
    m_log_test.add_attribute( "Level", boost::log::attributes::constant<std::wstring>( L"TEST" ) );

    std::wstring log_ini;
    std::wstring log_file_name;
    size_t log_rotation_size = 20 * 1024 * 1024;

    if ( vm.count( "log.ini" ) )
    {
        log_ini = vm["log.ini"].as<std::wstring>();
    }

    if ( vm.count( "log.file" ) )
    {
        log_file_name = vm["log.file"].as<std::wstring>();
    }

    if ( vm.count( "log.rotation-size" ) )
    {
        log_rotation_size = vm["log.rotation-size"].as<size_t>();
    }

    try
    {
        if ( false == log_ini.empty() )
        {
            std::ifstream is( log_ini.c_str() );
            boost::log::init_from_stream( is );
        }
        else
        {
            // set formatter
            m_formatter = expressions::stream
                << "\t"
                << expressions::format_date_time<boost::posix_time::ptime>( "TimeStamp", "%Y/%m/%d %H:%M:%S " )
                << expressions::if_( expressions::has_attr<int>( "Severity" ) )
                   [
                       expressions::stream << "[" << expressions::attr<int>( "Severity" ) << "] "
                   ]
                << expressions::if_( expressions::has_attr<std::wstring>( "Process" ) )
                   [
                       expressions::stream << "[" << expressions::attr<std::wstring>( "Process" ) << "] "
                   ]
                //<< "[" << expressions::attr<process_id>("ProcessID") << "] "
                //<< "[" << expressions::attr<thread_id>( "ThreadID" ) << "] "
                << expressions::format_named_scope( "Scope", keywords::format = "%c" )
                    << expressions::if_( expressions::has_attr<std::wstring>( "Channel" ) )
                    [
                        expressions::stream << "[" << expressions::attr<std::wstring>( "Channel" ) << "] "
                    ]
                << expressions::if_( expressions::has_attr<std::wstring>( "Level" ) )
                    [
                        expressions::stream << "[" << expressions::attr<std::wstring>( "Level" ) << "] "
                    ]
                << expressions::if_( expressions::has_attr<std::wstring>( "Tag" ) )
                   [
                       expressions::stream << "[" << expressions::attr<std::wstring>( "Tag" ) << "] "
                   ]
                << expressions::if_( expressions::has_attr<boost::posix_time::time_duration>( "Duration" ) )
                   [
                       expressions::stream << "[" << expressions::attr<boost::posix_time::time_duration>( "Duration" ) << "] "
                   ]
                << " "
                << expressions::message;

            if ( ( ! vm.count( "log.no-console" ) ) || ( vm["log.no-console"].as<std::wstring>() != L"true" ) )
            {
                // add sinks: console sink, file sink
                m_console_sink = add_console_log();
                m_console_sink->set_formatter( m_formatter );
            }

            if ( false == log_file_name.empty() )
            {
                m_file_sink = boost::log::add_file_log
                    (
                        keywords::file_name = log_file_name.c_str(),
                        keywords::rotation_size = log_rotation_size,
                        keywords::auto_flush = true
                    );

                m_file_sink->set_formatter( m_formatter );
                std::locale loc = boost::locale::generator()("en_US.UTF-8");
                m_file_sink->imbue( loc );
            }
        }

        ProgramOptions::connect_to_signal( &Log::update_option );

        add_common_attributes();
        //core::get()->add_global_attribute( "Scope", attributes::named_scope() );
        //core::get()->add_global_attribute( "Process", attributes::current_process_name() );
    }
    catch ( std::exception& e )
    {
        std::cout << __FUNCDNAME__ << " - " << e.what() << std::endl;
    }
}


void Log::update_option( const boost::program_options::variables_map& vm )
{
    std::vector<std::wstring> debug_levels;

    if ( vm.count( "log.levels" ) )
    {
        debug_levels = vm["log.levels"].as< std::vector<std::wstring> >();
    }

    std::sort( debug_levels.begin(), debug_levels.end() );
    std::sort( m_debug_levels.begin(), m_debug_levels.end() );

    if ( m_debug_levels == debug_levels )
    {
        return;
    }

    m_debug_levels = debug_levels;

    bool with_error = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"error" )    != m_debug_levels.end();
    bool with_info  = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"info" )     != m_debug_levels.end();
    bool with_debug = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"debug" )    != m_debug_levels.end();
    bool with_trace = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"trace" )    != m_debug_levels.end();
    bool with_test  = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"test" )     != m_debug_levels.end();
    bool with_all   = std::find( m_debug_levels.begin(), m_debug_levels.end(), L"*" )        != m_debug_levels.end();

    filter fltr =
        ( ! expressions::has_attr(debug_level_attribute) ) ||
        ( expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"ERROR" && ( with_error || with_all ) ) ||
        ( expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"INFO"  && ( with_info  || with_all ) ) ||
        ( expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"DEBUG" && ( with_debug || with_all ) ) ||
        ( expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"TRACE" && ( with_trace || with_all ) ) ||
        ( expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"TEST" &&  ( with_test  || with_all ) )
        ;

    if ( ( ! vm.count( "log.no-console" ) ) || ( vm["log.no-console"].as<std::wstring>() != L"true" ) )
    {
        if ( ! m_console_sink )
        {
            m_console_sink = add_console_log();
            m_console_sink->set_formatter( m_formatter );
        }
    }
    else
    {
        if ( m_console_sink )
        {
            m_console_sink.reset();
        }
    }

    if ( m_console_sink )
    {
        m_console_sink->set_filter( fltr );
    }

    if ( m_file_sink )
    {
        m_file_sink->set_filter( fltr );
    }
}
