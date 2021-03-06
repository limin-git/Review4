#include "stdafx.h"
#include "InputSender.h"


IInputSender& InputSender::key( WORD vk )
{
    return key( vk, false, false, false );
}


IInputSender& InputSender::Ctrl_key( WORD vk )
{
    return key( vk, true, false, false );
}


IInputSender& InputSender::Ctrl_Alt_key( WORD vk )
{
    return key( vk, true, true, false );
}


IInputSender& InputSender::Ctrl_Shift_key( WORD vk )
{
    return key( vk, true, false, true );
}


IInputSender& InputSender::Ctrl_Alt_Shift_key( WORD vk )
{
    return key( vk, true, true, true );
}


IInputSender& InputSender::Alt_key( WORD vk )
{
    return key( vk, false, true, false );
}


IInputSender& InputSender::Alt_Shift_key( WORD vk )
{
    return key( vk, false, true, true );
}


IInputSender& InputSender::Shift_key( WORD vk )
{
    return key( vk, false, false, true );
}


IInputSender& InputSender::string( const std::string& s )
{
    size_t size = 2 * s.size();
    std::vector<INPUT> vi( size );

    for ( size_t i = 0; i < size; ++i )
    {
        vi[i].type = INPUT_KEYBOARD;
        vi[i].ki.wVk = s[i/2];
        vi[i].ki.dwFlags = ( i % 2 ? 0 : KEYEVENTF_KEYUP );
        vi[i].ki.dwExtraInfo = NULL;
        vi[i].ki.wScan = 0;
        vi[i].ki.time = 0;
    }

    SendInput( size, &vi[0], sizeof(INPUT) );
    return *this;
}


IInputSender& InputSender::key( WORD vk, bool ctrl, bool alt, bool shift  )
{
    INPUT i;
    i.type = INPUT_KEYBOARD;
    i.ki.wVk = vk;
    i.ki.dwFlags = 0;
    i.ki.dwExtraInfo = NULL;
    i.ki.wScan = 0;
    i.ki.time = 0;

    if ( ctrl )  { i.ki.wVk = VK_CONTROL; SendInput( 1, &i, sizeof(INPUT) ); }
    if ( alt )   { i.ki.wVk = VK_MENU;    SendInput( 1, &i, sizeof(INPUT) ); }
    if ( shift ) { i.ki.wVk = VK_SHIFT;   SendInput( 1, &i, sizeof(INPUT) ); }

    i.ki.wVk = vk;
    SendInput( 1, &i, sizeof(INPUT) );
    i.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput( 1, &i, sizeof(INPUT) );

    if ( shift ) { i.ki.wVk = VK_SHIFT;   SendInput( 1, &i, sizeof(INPUT) ); }
    if ( alt )   { i.ki.wVk = VK_MENU;    SendInput( 1, &i, sizeof(INPUT) ); }
    if ( ctrl )  { i.ki.wVk = VK_CONTROL; SendInput( 1, &i, sizeof(INPUT) ); }

    return *this;
}
