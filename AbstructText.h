#pragma once
#include "IText.h"
#include "IDisableObserver.h"


struct AbstructText : IText,
                      IDisableObserver
{
public:
    AbstructText( const fs::path& file_path );
    ~AbstructText();
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const fs::path& get_file_path();
    virtual const KeyList& keys();
    virtual void add_observer( ITextObserver* observer );
    virtual void remove_observer( ITextObserver* observer );
    virtual void disabled( size_t key );

protected:

    fs::path m_file_path;
    std::wstring m_string;
    KeyList m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    boost::hash<std::wstring> m_hash;
    std::set<ITextObserver*> m_observers;
    bool m_parsed;
};
