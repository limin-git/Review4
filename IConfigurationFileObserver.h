#pragma once


struct IConfigurationFileObserver
{
    virtual ~IConfigurationFileObserver() {}
    virtual void options_changed( const boost::program_options::variables_map& vm, const boost::program_options::variables_map& old ) = 0;
};
