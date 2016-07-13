#pragma once
#include "IInputSender.h"


struct InputSender : IInputSender
{
public:

    virtual IInputSender& key( WORD vk );
    virtual IInputSender& Ctrl_key( WORD vk );
    virtual IInputSender& Ctrl_Alt_key( WORD vk );
    virtual IInputSender& Ctrl_Shift_key( WORD vk );
    virtual IInputSender& Ctrl_Alt_Shift_key( WORD vk );
    virtual IInputSender& Alt_key( WORD vk );
    virtual IInputSender& Alt_Shift_key( WORD vk );
    virtual IInputSender& Shift_key( WORD vk );
    virtual IInputSender& string( const std::string& s );

private:

    IInputSender& key( WORD vk, bool ctrl, bool alt, bool shft );
};
