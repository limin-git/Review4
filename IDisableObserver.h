#pragma once


struct IDisableObserver
{
    virtual ~IDisableObserver() {};
    virtual void disabled( size_t key ) = 0;
};
