#pragma once
namespace po = boost::program_options;


struct ICommandLine
{
    virtual ~ICommandLine() {}
    virtual void parse_command_line( int argc, wchar_t* argv[] ) = 0;
    virtual po::variables_map& variables_map() = 0;
    virtual po::options_description& options_description() = 0;
    static ICommandLine& instance();
};
