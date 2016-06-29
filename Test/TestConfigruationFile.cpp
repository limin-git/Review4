#include "stdafx.h"
#include "TestConfigruationFile.h"
#include "IFileChangeHandler.h"
#include "IFileChangeManager.h"
#include "WriteConsoleHelper.h"
#include "IConfigurationFileObserver.h"
#include "IConfigurationFile.h"
#include "CommandLine.h"



TestConfigruationFile::TestConfigruationFile()
{
    test1();
    //test2();
}


void TestConfigruationFile::test1()
{
    struct TestObserver : IConfigurationFileObserver
    {
        TestObserver( const std::string& option_name )
            : m_option_name( option_name )
        {
            po::options_description desc;
            desc.add_options()( m_option_name.c_str(), po::wvalue<std::wstring>(), m_option_name.c_str() );
            IConfigurationFile::instance().add_options_description( desc );
            IConfigurationFile::instance().add_observer( this );
        }

        virtual void options_changed( const boost::program_options::variables_map& old_map, const boost::program_options::variables_map& new_map )
        {
            stdcout << "options_changed: " << "\n";

            if ( new_map.count( m_option_name ) )
            {
                stdcout << m_option_name << " = "  << new_map[m_option_name].as<std::wstring>() << "\n";
            }
        }

        std::string m_option_name;
    };

    std::ofstream ofs( "test.txt" );
    ofs
        << "foo = hello, world" << std::endl
        << "bar = good, morning" << std::endl;
    ofs.close();

    wchar_t* argv[] = { L"place-holder", L"--config-file=test.txt" };
    int argc = sizeof( argv ) / sizeof( wchar_t* );
    g_command_line.parse_command_line( argc, argv );

    namespace po = boost::program_options;

    {
        TestObserver foo( "foo" );
        TestObserver bar( "bar" );
        _getch();
    }

    boost::filesystem::remove( "test.txt" );
}
