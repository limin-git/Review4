#pragma once


struct IInputSender
{
    virtual ~IInputSender() {}
    virtual IInputSender& key( WORD vk ) = 0;
    virtual IInputSender& Ctrl_key( WORD vk ) = 0;
    virtual IInputSender& Ctrl_Alt_key( WORD vk ) = 0;
    virtual IInputSender& Ctrl_Shift_key( WORD vk ) = 0;
    virtual IInputSender& Ctrl_Alt_Shift_key( WORD vk ) = 0;
    virtual IInputSender& Alt_key( WORD vk ) = 0;
    virtual IInputSender& Alt_Shift_key( WORD vk ) = 0;
    virtual IInputSender& Shift_key( WORD vk ) = 0;
    virtual IInputSender& string( const std::string& s ) = 0;
    static IInputSender& instance();
    static void remove();
};
