#pragma once


struct IInputHandler
{
    virtual ~IInputHandler() {}
};

typedef boost::function<void()> Callback;
