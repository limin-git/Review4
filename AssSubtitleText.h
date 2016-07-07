#pragma once
#include "IText.h"
#include "IDisableObserver.h"

namespace fs = boost::filesystem;


struct AssSubtitleText : IText,
                         IDisableObserver
{
public:

    AssSubtitleText( const fs::path& file_path );
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const fs::path& get_file_path();
    virtual const KeyList& keys();
    virtual void add_observer( ITextObserver* observer );
    virtual void remove_observer( ITextObserver* observer );
    virtual void disabled( size_t key );

private:

    void parse();
    size_t hash( std::wstring s );
    void notify();
    void trim_text( std::wstring& text );

public:

    KeyList m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    fs::path m_file_path;
    std::wstring m_string;
    boost::hash<std::wstring> m_hash;
    std::set<ITextObserver*> m_observers;
};
