#include "stdafx.h"
#include "Console.h"
#include "IConfigurationFile.h"
#include "Utility.h"
#include <conio.h>


Console::Console()
{
    m_hwnd = GetConsoleWindow();
    m_style = GetWindowLong( m_hwnd, GWL_STYLE );
    m_stdout = GetStdHandle( STD_INPUT_HANDLE );
    m_cout = CreateConsoleScreenBuffer( GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0 );
    SetStdHandle( STD_OUTPUT_HANDLE, m_cout );
    SetConsoleActiveScreenBuffer( m_cout );
    show_cursor( false );
    set_ctrl_handler();

    po::options_description options( "Console" );
    options.add_options()
        ( "console.font-name", po::wvalue<std::wstring>(), "console font name" )
        ( "console.font-size", po::wvalue<size_t>(), "console font size" )
        ( "console.width", po::value<size_t>(), "console layout: width" )
        ( "console.height", po::value<size_t>(), "console layout: height" )
        ( "console.color", po::wvalue<std::wstring>(), "console color" )
        ( "console.position", po::wvalue<std::wstring>(), "position(left,top)" )
        ( "console.disable-system-menu", po::wvalue<std::wstring>(), "disable system menu?)" )
        ;
    IConfigurationFile::instance().add_options_description( options ).add_observer( this );
}


Console::~Console()
{
    SetStdHandle( STD_OUTPUT_HANDLE, m_stdout );
    SetConsoleActiveScreenBuffer( m_stdout );
    CloseHandle( m_cout );
    SetWindowLong( m_hwnd, GWL_STYLE, m_style );
}


IConsole& Console::cls()
{
    DWORD written = 0;
    COORD coord = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    std::wstring buf( csbi.dwSize.X * csbi.dwSize.Y, L' ' );
    WriteConsoleOutputCharacter( m_cout, buf.c_str(), static_cast<DWORD>(buf.size()), coord, &written );
    SetConsoleCursorPosition( m_cout, coord );
    return *this;
}


IConsole& Console::write( const std::string& s )
{
    WriteConsoleA( m_cout, s.c_str(), s.size(), 0, 0 );
    return *this;
}


IConsole& Console::write( const std::wstring& ws )
{
    WriteConsole( m_cout, ws.c_str(), ws.size(), 0, 0 );
    return *this;
}


IConsole& Console::write_center( const std::string& s )
{
    DWORD written = 0;
    COORD coord = calculate_center_coord( s.size() );
    WriteConsoleOutputCharacterA( m_cout, s.c_str(), s.size(), coord, &written );
    return *this;
}


IConsole& Console::write_center( const std::wstring& ws )
{
    DWORD written = 0;
    COORD coord = calculate_center_coord( WideCharToMultiByte( CP_ACP, 0, ws.c_str(), ws.size(), 0, 0, 0, 0 ) );
    WriteConsoleOutputCharacter( m_cout, ws.c_str(), static_cast<DWORD>(ws.size()), coord, &written );
    return *this;
}


IConsole& Console::title( const std::wstring& title )
{
    SetConsoleTitle( title.c_str() );
    return *this;
}


COORD Console::calculate_center_coord( size_t string_length )
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo( m_cout, &csbi );
    size_t window_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    size_t window_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if ( 0 == ( window_height % 2 ) )
    {
        --window_height;
    }

    size_t pos = ( window_width * window_height - string_length ) / 2;
    COORD coord = { pos % window_width, pos / window_width };
    return coord;
}


void Console::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<std::wstring>( "console.font-name", vm, old ) )
    {
        set_font_face_name( vm["console.font-name"].as<std::wstring>() );
    }

    if ( Utility::updated<size_t>( "console.font-size", vm, old ) )
    {
        set_font_size( vm["console.font-size"].as<size_t>() );
    }

    if ( Utility::updated<size_t>( "console.width", vm, old ) )
    {
        set_width( vm["console.width"].as<size_t>() );
    }

    if ( Utility::updated<size_t>( "console.height", vm, old ) )
    {
        set_height( vm["console.height"].as<size_t>() );
    }

    if ( Utility::updated<std::wstring>( "console.color", vm, old ) )
    {
        WORD color = 0;
        std::wstringstream strm( vm["console.color"].as<std::wstring>() );
        strm >> std::hex >> color;
        if ( 0 == color )
        {
            color = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        }
        set_color( color);
    }

    if ( Utility::updated<std::wstring>( "console.position", vm, old ) )
    {
        std::wstring s = vm["console.position"].as<std::wstring>();
        size_t p = s.find( L"," );

        if ( p != std::wstring::npos )
        {
            short left = boost::lexical_cast<short>( s.substr( 0, p ) );
            short top = boost::lexical_cast<short>( s.substr( p + 1 ) );
            set_position( left, top );
        }
    }

    if ( Utility::updated<std::wstring>( "console.disable-system-menu", vm, old ) )
    {
        disable_system_buttons( L"true" == vm["console.disable-system-menu"].as<std::wstring>() );
    }
}


void Console::set_font_face_name( const std::wstring& name )
{
    if ( name !=  L"新宋体" && name != L"Consolas" && name != L"Lucida Console"  )
    {
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

    if ( csbi.wAttributes != color )
    {
        csbi.wAttributes = color;
        SetConsoleTextAttribute( m_cout, csbi.wAttributes );
        FillConsoleOutputAttribute( m_cout, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &written );
    }
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


void Console::disable_system_buttons( bool disable )
{
    LONG style = GetWindowLong( m_hwnd, GWL_STYLE );

    if ( disable )
    {
        style &= ~(WS_SYSMENU|WS_SIZEBOX); // no menu, no sizing
    }
    else
    {
        style |= (WS_SYSMENU|WS_SIZEBOX);
    }

    SetWindowLong( m_hwnd, GWL_STYLE, style );
}


void Console::disable_system_buttons2()
{
    HWND w = GetConsoleWindow();
    HMENU m = GetSystemMenu( w, FALSE );
    DeleteMenu( m, SC_CLOSE , MF_BYCOMMAND );
    DeleteMenu( m, SC_MINIMIZE , MF_BYCOMMAND );
    DeleteMenu( m, SC_MAXIMIZE , MF_BYCOMMAND );
    DrawMenuBar( w );
}


void Console::set_ctrl_handler()
{
    struct Hide
    {
        static int WINAPI ctrl_handler( DWORD ctrl_type )
        {
            DWORD ctrl_types[] = { CTRL_C_EVENT, CTRL_BREAK_EVENT, CTRL_CLOSE_EVENT, CTRL_LOGOFF_EVENT, CTRL_SHUTDOWN_EVENT };
            const char* ctrl_info[] = { "Ctrl + C", "Ctrl + BREAK", "CLOSE EVENT", "LOGOFF EVENT",  "SHUTDOWN EVENT" };
            IConsole::instance().write( ctrl_info[ctrl_type] );
            return TRUE;
        }
    };

    SetConsoleCtrlHandler( Hide::ctrl_handler, TRUE );
}


void Console::set_position( short left, short top )
{
    WINDOWINFO info;
    info.cbSize = sizeof(WINDOWINFO);
    GetWindowInfo( m_hwnd, &info );
    RECT r = info.rcWindow;
    long width = r.right - r.left;
    long height = r.bottom - r.top;
    SetWindowPos( m_hwnd, NULL, left, top, width, height, 0 );
}
