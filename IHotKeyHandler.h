#pragma once


struct IHotKeyHandler
{
    virtual ~IHotKeyHandler() {}
};

typedef boost::function<void()> Function;
