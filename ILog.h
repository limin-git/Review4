#pragma once


struct ILog
{
    virtual ~ILog() {}
    static ILog& instance();
    static void remove();
};

namespace lg = boost::log;

extern lg::sources::wlogger g_log_always;
extern lg::sources::wlogger g_log_error;
extern lg::sources::wlogger g_log_info;
extern lg::sources::wlogger g_log_debug;
extern lg::sources::wlogger g_log_trace;
extern lg::sources::wlogger g_log_test;
extern bool g_with_error;
extern bool g_with_info;
extern bool g_with_debug;
extern bool g_with_trace;
extern bool g_with_test;
extern bool g_with_always;

#define source_info __FUNCTION__ << ":" << __LINE__ << " - "
#define log_allways if ( g_with_always )BOOST_LOG(g_log_always) << source_info
#define log_error   if ( g_with_error ) BOOST_LOG(g_log_error)  << source_info
#define log_info    if ( g_with_info )  BOOST_LOG(g_log_info)   << source_info
#define log_debug   if ( g_with_debug)  BOOST_LOG(g_log_debug)  << source_info
#define log_trace   if ( g_with_trace)  BOOST_LOG(g_log_trace)  << source_info
#define log_test    if ( g_with_test)   BOOST_LOG(g_log_test)   << source_info
