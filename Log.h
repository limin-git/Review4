#pragma once
#include "ILog.h"
#include "IConfigurationFileObserver.h"

namespace lg = boost::log;
namespace po = boost::program_options;


struct Log : ILog, 
             IConfigurationFileObserver
{
public:

    Log();
    ~Log();
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:
    
    void setup_sink( const std::wstring& file_nam, size_t rotation_size );

private:

    lg::formatter m_formatter;
    boost::shared_ptr< lg::sinks::synchronous_sink<lg::sinks::text_file_backend> > m_sink;
    std::wstring m_levels;
    po::options_description m_options_description;
};
