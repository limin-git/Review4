#pragma once


struct IText;

struct ITextObserver
{
    virtual ~ITextObserver() {}
    virtual void text_changed( IText* text ) = 0;
};
