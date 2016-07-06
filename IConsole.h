#pragma once


struct IConsole
{
    virtual ~IConsole() {}
    virtual IConsole& cls() = 0;
    virtual IConsole& write( const std::string& s ) = 0;
    virtual IConsole& write( const std::wstring& ws ) = 0;
    virtual IConsole& write_center( const std::string& s ) = 0;
    virtual IConsole& write_center( const std::wstring& ws ) = 0;
    virtual IConsole& title( const std::wstring& title ) = 0;
    static IConsole& instance();
    static void remove();
};
