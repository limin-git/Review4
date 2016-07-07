#pragma once


struct IInputSender
{
    virtual ~IInputSender() {}
    virtual IInputSender& key( WORD vk, bool ctrl = false, bool alt = false, bool shft = false ) = 0;
    virtual IInputSender& string( const std::string& s ) = 0; 
    static IInputSender& instance();
    static void remove();
};
