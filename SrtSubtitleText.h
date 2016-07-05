#pragma once
#include "IText.h"
#include "IDisableObserver.h"

namespace fs = boost::filesystem;


struct SrtSubtitleText : IText,
                         IDisableObserver
{
public:

    SrtSubtitleText( const fs::path& file_path );
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const std::set<size_t>& keys();
    virtual void add_observer( ITextObserver* observer );
    virtual void remove_observer( ITextObserver* observer );
    virtual void disabled( size_t key );

private:

    void parse();
    size_t hash( std::wstring s );
    void notify();

public:

    std::set<size_t> m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    fs::path m_file_path;
    std::wstring m_string;
    boost::hash<std::wstring> m_hash;
    std::set<ITextObserver*> m_observers;
};
