#pragma once


struct ISlideshow
{
    virtual ~ISlideshow() {}
    virtual bool show() = 0;
};

typedef boost::shared_ptr<ISlideshow> ISlideshowPtr;
typedef std::list<ISlideshowPtr> ISlideshowPtrList;
