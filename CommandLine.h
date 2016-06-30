#pragma once
#include "ICommandLine.h"


struct CommandLine : ICommandLine
{
public:

    CommandLine();
    virtual void parse_command_line( int argc, wchar_t* argv[] );
    virtual boost::program_options::variables_map& get_variables_map() { return m_variables_map; }
    virtual boost::program_options::options_description& get_options_description() { return m_options_description; }

private:

    boost::program_options::variables_map m_variables_map;
    boost::program_options::options_description m_options_description;
};
