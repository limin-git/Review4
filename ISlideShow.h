#pragma once


struct ISlideshow
{
    virtual ~ISlideshow() {}
    virtual bool show() = 0;
    virtual bool show_all() { return true; };
    virtual size_t key() = 0;
    virtual const std::wstring& key_string() = 0;
    virtual void clear_state() = 0;
    virtual bool empty() = 0;
};

typedef boost::shared_ptr<ISlideshow> ISlideshowPtr;
typedef std::list<ISlideshowPtr> ISlideshowPtrList;
