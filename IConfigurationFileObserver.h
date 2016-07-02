#pragma once

namespace po = boost::program_options;


struct IConfigurationFileObserver
{
    virtual ~IConfigurationFileObserver() {}
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old ) = 0;
};
