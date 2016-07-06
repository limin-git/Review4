#pragma once


struct IHotKeyHandler
{
    virtual ~IHotKeyHandler() {}
    int unique_id();
};

typedef boost::function<void()> HotKeyCallback;
