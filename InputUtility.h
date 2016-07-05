#pragma once


namespace Utility
{
    void send_input( WORD vk );
    void send_input( const std::string& str );
    void send_input_ctrl( const char ch );
    void send_input_enter();
}
