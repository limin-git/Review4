#include "stdafx.h"
#include "TestConfigruationFile.h"
#include "IFileChangeHandler.h"
#include "IFileChangeManager.h"
#include "WriteConsoleHelper.h"
#include "IConfigurationFileObserver.h"
#include "IConfigurationFile.h"
#include "ICommandLine.h"



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

        virtual void options_changed( const boost::program_options::variables_map& vm, const boost::program_options::variables_map& old )
        {
            if ( vm.count( m_option_name ) )
            {
                if ( old.empty() || ( vm[m_option_name].as<std::wstring>() != old[m_option_name].as<std::wstring>() ) )
                {
                    stdcout << "options_changed: " << "\n";
                    stdcout << m_option_name << " = "  << vm[m_option_name].as<std::wstring>() << "\n";
                }
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
    ICommandLine::instance().parse_command_line( argc, argv );

    namespace po = boost::program_options;

    {
        TestObserver foo( "foo" );
        TestObserver bar( "bar" );
        _getch();
    }

    boost::filesystem::remove( "test.txt" );
}
