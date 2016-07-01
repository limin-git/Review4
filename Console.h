#pragma once
#include "IConsole.h"


struct Console : IConsole
{
public:

    Console();
    ~Console();
    virtual void cls();
    virtual void write( const std::string& s );
    virtual void write( const std::wstring& ws );

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

private:
    
    HANDLE m_cout;
    HANDLE m_stdout;
};
