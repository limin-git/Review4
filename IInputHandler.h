#pragma once


struct IInputHandler
{
    virtual ~IInputHandler() {}
};

typedef boost::function<void()> Callback;
typedef std::vector<Callback> CallbackList;
