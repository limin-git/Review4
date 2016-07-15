#pragma once
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup.hpp>
#include <boost/locale.hpp>


struct ILog
{
    virtual ~ILog() {}
    static ILog& instance();
};

enum { ERROR_LOGGER, INFO_LOGGER, DEBUG_LOGGER, TRACE_LOGGER, TEST_LOGGER, LOG_NUM };
extern const wchar_t* g_log_keywords[];
extern boost::log::sources::wlogger* g_boost_wlogger[];

#define source_info __FUNCTION__ << ":" << __LINE__ << " - "
#define log_error   if ( g_boost_wlogger[ERROR_LOGGER] )    BOOST_LOG(*g_boost_wlogger[ERROR_LOGGER])  << source_info
#define log_info    if ( g_boost_wlogger[INFO_LOGGER] )     BOOST_LOG(*g_boost_wlogger[INFO_LOGGER])   << source_info
#define log_debug   if ( g_boost_wlogger[DEBUG_LOGGER] )    BOOST_LOG(*g_boost_wlogger[DEBUG_LOGGER])  << source_info
#define log_trace   if ( g_boost_wlogger[TRACE_LOGGER] )    BOOST_LOG(*g_boost_wlogger[TRACE_LOGGER])  << source_info
#define log_test    if ( g_boost_wlogger[TEST_LOGGER] )     BOOST_LOG(*g_boost_wlogger[TEST_LOGGER])   << source_info
