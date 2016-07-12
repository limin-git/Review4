#pragma once
#include "ICommandLine.h"


struct CommandLine : ICommandLine
{
public:

    CommandLine();
    virtual void parse_command_line( int argc, wchar_t* argv[] );
    virtual po::variables_map& variables_map() { return m_variables_map; }
    virtual po::options_description& options_description() { return m_options_description; }

private:

    po::variables_map m_variables_map;
    po::options_description m_options_description;
};
