#pragma once
#include "IInputSender.h"


struct InputSender : IInputSender
{
public:

    InputSender();
    ~InputSender();
    virtual IInputSender& key( WORD vk, bool ctrl = false, bool alt = false, bool shft = false );
    virtual IInputSender& string( const std::string& s );

private:

};
