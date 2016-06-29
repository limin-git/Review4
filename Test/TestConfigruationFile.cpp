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
        virtual void options_changed( const boost::program_options::variables_map& old_map, const boost::program_options::variables_map& new_map )
        {
            stdcout << "options_changed: " << "\n";
        }
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
        TestObserver observer;
        po::options_description desc;
        desc.add_options()( "foo", "foo" );
        IConfigurationFile::instance().add_options_description( desc );
        IConfigurationFile::instance().add_observer( &observer );
        _getch();
    }

    boost::filesystem::remove( "test.txt" );
}
