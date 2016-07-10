#include "stdafx.h"
#include "Log.h"
#include "IConfigurationFile.h"

BOOST_LOG_ATTRIBUTE_KEYWORD(debug_level_attribute, "Level", std::wstring)
lg::sources::wlogger* g_boost_wlogger[ LOG_NUM ] = { 0 };
const wchar_t* g_log_keywords[] = { L"ERROR", L"INFO", L"DEBUG", L"TRACE", L"TEST" };


Log::Log()
    : m_options_description( "Log" )
{
    m_options_description.add_options()
        ( "log.file", po::wvalue<std::wstring>(), "log file name." )
        ( "log.rotation-size", po::value<size_t>()->default_value( 20 * 1024 * 1024 ), "log rotation size." )
        ( "log.levels", po::wvalue<std::wstring>()->multitoken(), "error,info,debug,trace,*" )
        ;
    m_configuration->add_options_description( m_options_description ).add_observer( this );
}


Log::~Log()
{
    for ( size_t i = 0; i < LOG_NUM; ++i )
    {
        delete g_boost_wlogger[i];
        g_boost_wlogger[i] = NULL;
    }

    boost::log::core::get()->remove_sink( m_sink );
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

        if ( vm.count( "log.file" ) )
        {
            file_name = vm["log.file"].as<std::wstring>();
        }

        if ( file_name.empty() )
        {
            return;
        }

        setup_sink( file_name, vm["log.rotation-size"].as<size_t>() );
    }

    if ( !m_sink )
    {
        return;
    }

    if ( vm.count( "log.levels" ) )
    {
        std::wstring levels = vm["log.levels"].as<std::wstring>();
        std::transform( levels.begin(), levels.end(), levels.begin(), ::towupper );

        if ( levels.empty() )
        {
            levels = L"INFO,ERROR";
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

    for ( size_t i = 0; i < sizeof(g_log_keywords) / sizeof(wchar_t*); ++i )
    {
        if ( m_levels.find( g_log_keywords[i] ) != std::wstring::npos )
        {
            if ( g_boost_wlogger[i] == NULL )
            {
                g_boost_wlogger[i] = new lg::sources::wlogger;
                g_boost_wlogger[i]->add_attribute( "Level", lg::attributes::constant<std::wstring>( g_log_keywords[i] ) );
            }
        }
        else
        {
            if ( g_boost_wlogger[i] != NULL )
            {
                delete g_boost_wlogger[i];
                g_boost_wlogger[i] = NULL;
            }
        }
    }
}
