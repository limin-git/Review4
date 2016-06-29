#pragma once


struct CommandLine
{
    CommandLine();
    void parse_command_line( int argc, wchar_t* argv[] );
    boost::program_options::variables_map m_vm;
    boost::program_options::options_description m_desc;
};


extern CommandLine g_command_line;
