#include "stdafx.h"
#include "Console.h"


Console::Console()
{
    m_stdout = GetStdHandle( STD_INPUT_HANDLE );
    m_cout = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0 );
    SetStdHandle( STD_OUTPUT_HANDLE, m_cout );
    SetConsoleActiveScreenBuffer( m_cout );
    show_cursor( false );
    disable_system_buttons();
}


Console::~Console()
{
    SetStdHandle( STD_OUTPUT_HANDLE, m_stdout );
    SetConsoleActiveScreenBuffer( m_stdout );
    CloseHandle( m_cout );
}


void Console::cls()
{
    DWORD written = 0;
    COORD coord = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    std::wstring buf( csbi.dwSize.X * csbi.dwSize.Y, L' ' );
    WriteConsoleOutputCharacter( m_cout, buf.c_str(), buf.size(), coord, &written );
    SetConsoleCursorPosition( m_cout, coord );
}


void Console::write( const std::string& s )
{
    WriteConsoleA( m_cout, s.c_str(), s.size(), 0, 0 );
}


void Console::write( const std::wstring& ws )
{
    WriteConsole( m_cout, ws.c_str(), ws.size(), 0, 0 );
}


void Console::update_option( const boost::program_options::variables_map& vm )
{
#if 0
    static OptionUpdateHelper option_helper;

    if ( option_helper.update_one_option<std::wstring>( system_font_face_name, vm ) )
    {
        std::wstring font_face_name = option_helper.get_value<std::wstring>( system_font_face_name );
        set_font_face_name( font_face_name );
        //LOG_DEBUG << "font-face-name: " << font_face_name;
    }

    if ( option_helper.update_one_option<SHORT>( system_font_size, vm ) )
    {
        SHORT font_size = option_helper.get_value<SHORT>( system_font_size );
        set_font_size( font_size );
        //LOG_DEBUG << "font-size: " << font_size;
    }

    if ( option_helper.update_one_option<SHORT>( system_console_width, vm ) )
    {
        SHORT width = option_helper.get_value<SHORT>( system_console_width );
        set_width( width );
        //LOG_DEBUG << "console-width: " << width;
    }

    if ( option_helper.update_one_option<SHORT>( system_console_height, vm ) )
    {
        SHORT height = option_helper.get_value<SHORT>( system_console_height );
        set_height( height );
        //LOG_DEBUG << "console-height: " << height;
    }

    if ( option_helper.update_one_option<std::wstring>( system_console_color, vm ) )
    {
        std::wstring color_str = option_helper.get_value<std::wstring>( system_console_color );
        WORD color = 0;
        std::wstringstream strm;
        strm << color_str;
        strm >> std::hex >> color;

        if ( 0 == color )
        {
            color = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
            //LOG_ERROR << "wrong console-color: " << color_str;
        }

        set_color( color );
        //LOG_DEBUG << "console-color: " << color;
    }
#endif
}


void Console::set_font_face_name( const std::wstring& name )
{
    if ( name !=  L"新宋体" && name != L"Consolas" && name != L"Lucida Console"  )
    {
        //LOG_ERROR << "wrong font-face-name: " << name;
        return;
    }

    CONSOLE_FONT_INFOEX f;
    f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
    GetCurrentConsoleFontEx( m_cout, FALSE, &f );
    if ( f.FaceName != name )
    {
        SetConsoleOutputCP( name == L"新宋体" ? 936 : 936 );
        wcscpy_s( f.FaceName, name.c_str() );
        SetCurrentConsoleFontEx( m_cout, FALSE, &f );
    }
}


void Console::set_font_size( SHORT font_size )
{
    CONSOLE_FONT_INFOEX f;
    f.cbSize = sizeof( CONSOLE_FONT_INFOEX );
    GetCurrentConsoleFontEx( m_cout, FALSE, &f );
    if ( f.dwFontSize.Y != font_size )
    {
        f.dwFontSize.Y = font_size;
        SetCurrentConsoleFontEx( m_cout, FALSE, &f );
    }
}


void Console::set_width( SHORT width )
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    COORD cur_size = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };

    if ( cur_size.X != width )
    {
        set_size( width, cur_size.Y );
    }
}


void Console::set_height( SHORT height )
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    COORD cur_size = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };

    if ( cur_size.Y != height )
    {
        set_size( cur_size.X, height );
    }
}


void Console::set_size( SHORT width, SHORT height )
{
    COORD coord = { 0, 0 };
    COORD size = { width, height };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    COORD cur_size = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };

    if ( (size.X != cur_size.X) || (size.Y != cur_size.Y) )
    {
        COORD tmp_buf_size = { std::max<int>(cur_size.X, width), std::max<int>(cur_size.Y, height) };
        SetConsoleScreenBufferSize( m_cout, tmp_buf_size );

        SMALL_RECT window = csbi.srWindow;
        window.Right = window.Left + width - 1;
        window.Bottom = window.Top + height - 1;
        SetConsoleWindowInfo( m_cout, TRUE, &window );

        SetConsoleScreenBufferSize( m_cout, size );
    }
}


void Console::set_color( WORD color )
{
    DWORD written = 0;
    COORD coord = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    csbi.wAttributes = color;
    SetConsoleTextAttribute( m_cout, csbi.wAttributes );
    FillConsoleOutputAttribute( m_cout, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &written );
}


void Console::show_cursor( BOOL visible )
{
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo( m_cout, &cursor_info );

    if ( visible != cursor_info.bVisible )
    {
        cursor_info.bVisible = visible;
        SetConsoleCursorInfo( m_cout, &cursor_info );
    }
}


void Console::disable_system_buttons()
{
    HWND w = GetConsoleWindow();
    HMENU m = GetSystemMenu( w, FALSE );
    DeleteMenu( m, SC_CLOSE , MF_BYCOMMAND );
    DeleteMenu( m, SC_MINIMIZE , MF_BYCOMMAND );
    DeleteMenu( m, SC_MAXIMIZE , MF_BYCOMMAND );
    DrawMenuBar( w );
}
