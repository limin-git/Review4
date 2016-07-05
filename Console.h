#pragma once
#include "IConsole.h"
#include "IConfigurationFileObserver.h"


struct Console : IConsole,
                 IConfigurationFileObserver
{
public:

    Console();
    ~Console();
    virtual IConsole& cls();
    virtual IConsole& write( const std::string& s );
    virtual IConsole& write( const std::wstring& ws );
    virtual IConsole& write_center( const std::string& s );
    virtual IConsole& write_center( const std::wstring& ws );

public:

    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:

    void set_font_face_name( const std::wstring& name );
    void set_font_size( SHORT font_size );
    void set_width( SHORT width );
    void set_height( SHORT height );
    void set_size( SHORT col, SHORT row );
    void set_color( WORD color );
    void show_cursor( BOOL visible = true );
    void disable_system_buttons();
    void update_option( const boost::program_options::variables_map& vm );
    void set_ctrl_handler();
    COORD calculate_center_coord( size_t string_length );

private:

    HANDLE m_cout;
    HANDLE m_stdout;
};
