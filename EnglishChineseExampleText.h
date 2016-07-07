#pragma once
#include "IText.h"
#include "IFileChangeHandler.h"
#include "IDisableObserver.h"


struct EnglishChineseExampleText : IText,
                                   IFileChangeHandler,
                                   IDisableObserver
{
public:

    EnglishChineseExampleText( const fs::path& file_path );
    ~EnglishChineseExampleText();
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const fs::path& get_file_path();
    virtual const KeyList& keys();
    virtual void add_observer( ITextObserver* observer );
    virtual void remove_observer( ITextObserver* observer );
    virtual void last_write_time_changed( const fs::path& file );
    virtual void disabled( size_t key );

private:

    bool reload();
    size_t hash( std::wstring s );
    void notify();

private:

    KeyList m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    fs::path m_file_path;
    std::wstring m_string;
    boost::hash<std::wstring> m_hash;
    std::set<ITextObserver*> m_observers;
};
