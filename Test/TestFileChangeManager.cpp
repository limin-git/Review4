#include "stdafx.h"
#include "TestFileChangeManager.h"
#include "IFileChangeHandler.h"
#include "IFileChangeManager.h"
#include "WriteConsoleHelper.h"



TestFileChangeManager::TestFileChangeManager()
{
    //test1();
    test2();
}


void TestFileChangeManager::test1()
{
    struct TestHandler : IFileChangeHandler
    {
        virtual void last_write_time_changed( const boost::filesystem::path& file )
        {
            stdcout << "handle change: " << file.wstring() << "\n";
        }
    };

    TestHandler handler;
    IFileChangeManager::instance().add_handler( L"C:\\Temp\\aa.txt", &handler );
    _getch();
    IFileChangeManager::instance().remove_handler( L"C:\\Temp\\aa.txt", &handler );
    _getch();
}


void TestFileChangeManager::test2()
{
    struct TestHandler : IFileChangeHandler
    {
        virtual void last_write_time_changed( const boost::filesystem::path& file )
        {
            stdcout << "handle change: " << file.wstring() << "\n";
        }
    };

    TestHandler handler1;
    IFileChangeManager::instance().add_handler( L"C:\\Temp\\New folder (2)\\x.txt", &handler1 );
    _getch();
    TestHandler handler2;
    IFileChangeManager::instance().add_handler( L"C:\\Temp\\aa.txt", &handler2 );
    _getch();
    IFileChangeManager::instance().remove_handler( L"C:\\Temp\\aa.txt", &handler2 );
    _getch();
    IFileChangeManager::instance().remove_handler( L"C:\\Temp\\New folder (2)\\x.txt", &handler1 );
}
