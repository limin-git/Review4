#pragma once


struct IConsole
{
    virtual ~IConsole() {}
    virtual void cls() = 0;
    virtual void write( const std::string& s ) = 0;
    virtual void write( const std::wstring& ws ) = 0;
    virtual void write_center( const std::string& s ) = 0;
    virtual void write_center( const std::wstring& ws ) = 0;
    static IConsole& instance();
    static void remove();
};
