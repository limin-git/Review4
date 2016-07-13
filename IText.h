#pragma once
#include "ISlideshow.h"
#include "ITextObserver.h"
namespace fs = boost::filesystem;
typedef std::list<size_t> KeyList;


struct IText
{
    virtual ~IText() {}
    virtual ISlideshowPtr slideshow( size_t hash_key ) = 0;
    virtual const fs::path& get_file_path() = 0;
    virtual const KeyList& keys() = 0;
    virtual void add_observer( ITextObserver* observer ) = 0;
    virtual void remove_observer( ITextObserver* observer ) = 0;
    virtual bool parse_text() = 0;
    static IText& instance();
};
