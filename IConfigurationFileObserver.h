#pragma once


struct IConfigurationFileObserver
{
    virtual ~IConfigurationFileObserver() {}
    virtual void options_changed( const boost::program_options::variables_map& old_map, const boost::program_options::variables_map& new_map ) = 0;
};
