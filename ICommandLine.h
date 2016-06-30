#pragma once


struct ICommandLine
{
    virtual ~ICommandLine() {}
    virtual void parse_command_line( int argc, wchar_t* argv[] ) = 0;
    virtual boost::program_options::variables_map& get_variables_map() = 0;
    virtual boost::program_options::options_description& get_options_description() = 0;
    static ICommandLine& instance();
    static void remove();
};
