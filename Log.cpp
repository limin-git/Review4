#include "stdafx.h"
#include "Log.h"
#include "IConfigurationFile.h"

BOOST_LOG_ATTRIBUTE_KEYWORD(debug_level_attribute, "Level", std::wstring)

lg::sources::wlogger g_log_always;
lg::sources::wlogger g_log_error;
lg::sources::wlogger g_log_info;
lg::sources::wlogger g_log_debug;
lg::sources::wlogger g_log_trace;
lg::sources::wlogger g_log_test;

bool g_with_error   = false;
bool g_with_info    = false;
bool g_with_debug   = false;
bool g_with_trace   = false;
bool g_with_test    = false;
bool g_with_always  = false;


Log::Log()
    : m_options_description( "Log" )
{
    m_options_description.add_options()
        ( "log.file", po::wvalue<std::wstring>(), "log file name." )
        ( "log.rotation-size", po::value<size_t>()->default_value( 20 * 1024 * 1024 ), "log rotation size." )
        ( "log.levels", po::wvalue<std::wstring>()->multitoken(), "error,info,debug,trace,*" )
        ;

    g_log_error.add_attribute( "Level", lg::attributes::constant<std::wstring>( L"ERROR" ) );
    g_log_info. add_attribute( "Level", lg::attributes::constant<std::wstring>( L"INFO" ) );
    g_log_debug.add_attribute( "Level", lg::attributes::constant<std::wstring>( L"DEBUG" ) );
    g_log_trace.add_attribute( "Level", lg::attributes::constant<std::wstring>( L"TRACE" ) );
    g_log_test. add_attribute( "Level", lg::attributes::constant<std::wstring>( L"TEST" ) );

    IConfigurationFile::instance()
        .add_options_description( m_options_description )
        .add_observer( this );
}


Log::~Log()
{
    m_formatter.reset();
    m_sink.reset();
}


void Log::setup_sink( const std::wstring& file_nam, size_t rotation_size )
{
    try
    {
        std::locale loc = boost::locale::generator()("en_US.UTF-8");
        m_formatter = lg::expressions::stream
            << "\t"
            << lg::expressions::format_date_time<boost::posix_time::ptime>( "TimeStamp", "%Y/%m/%d %H:%M:%S " )
            //<< "[" << lg::expressions::attr<lg::thread_id>( "ThreadID" ) << "] "
            << lg::expressions::if_( lg::expressions::has_attr<std::wstring>( "Level" ) )
            [
                lg::expressions::stream << "[" << lg::expressions::attr<std::wstring>( "Level" ) << "] "
            ]
        << " "
        << lg::expressions::message;

        m_sink = boost::log::add_file_log
            (
                lg::keywords::file_name = file_nam.c_str(),
                lg::keywords::rotation_size = rotation_size,
                lg::keywords::auto_flush = true
            );

        m_sink->set_formatter( m_formatter );
        m_sink->imbue( loc );
        lg::add_common_attributes();
        g_with_always = true;
    }
    catch ( std::exception& e )
    {
        std::cout << __FUNCDNAME__ << " - " << e.what() << std::endl;
    }
}


void Log::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( !m_sink )
    {
        std::wstring file_name;
        size_t rotation_size = 20 * 1024 * 1024;

        if ( vm.count( "log.file" ) )
        {
            file_name = vm["log.file"].as<std::wstring>();
        }

        if ( file_name.empty() )
        {
            return;
        }

        if ( vm.count( "log.rotation-size" ) )
        {
            rotation_size = vm["log.rotation-size"].as<size_t>();
        }

        setup_sink( file_name, rotation_size );
    }

    if ( !m_sink )
    {
        return;
    }

    if ( vm.count( "log.levels" ) )
    {
        std::wstring levels = vm["log.levels"].as<std::wstring>();
        std::transform( levels.begin(), levels.end(), levels.begin(), ::towlower );

        if ( levels.find( L"*" ) != std::wstring::npos )
        {
            levels = L"*";
        }

        if ( levels.empty() )
        {
            levels = L"info,error";
        }

        if ( levels == m_levels )
        {
            return;
        }

        std::wstring t1 = levels;
        std::wstring t2 = m_levels;
        std::sort( t1.begin(), t1.end() );
        std::sort( t2.begin(), t2.end() );

        if ( t1 == t2 )
        {
            return;
        }

        m_levels = levels;
    }

    bool with_all   = m_levels.find( L"*" )   != std::wstring::npos;
    g_with_error = with_all || ( m_levels.find( L"error" )  != std::wstring::npos );
    g_with_info  = with_all || ( m_levels.find( L"info" )   != std::wstring::npos );
    g_with_debug = with_all || ( m_levels.find( L"debug" )  != std::wstring::npos );
    g_with_trace = with_all || ( m_levels.find( L"trace" )  != std::wstring::npos );
    g_with_test  = with_all || ( m_levels.find( L"test" )   != std::wstring::npos );

    lg::filter filter =
        ( ! lg::expressions::has_attr(debug_level_attribute) ) ||
        ( lg::expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"ERROR" && ( g_with_error ) ) ||
        ( lg::expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"INFO"  && ( g_with_info  ) ) ||
        ( lg::expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"DEBUG" && ( g_with_debug ) ) ||
        ( lg::expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"TRACE" && ( g_with_trace ) ) ||
        ( lg::expressions::has_attr(debug_level_attribute) && debug_level_attribute == L"TEST"  && ( g_with_test  ) )
        ;

    m_sink->set_filter( filter );
}
