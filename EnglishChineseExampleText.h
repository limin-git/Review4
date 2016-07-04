#pragma once
#include "IText.h"
#include "IFileChangeHandler.h"
#include "IDisableObserver.h"


struct EnglishChineseExampleText : IText,
                                   IFileChangeHandler,
                                   IDisableObserver
{
public:

    EnglishChineseExampleText( const boost::filesystem::path& file_path );
    ~EnglishChineseExampleText();
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const std::set<size_t>& keys();
    virtual void add_observer( ITextObserver* observer );
    virtual void remove_observer( ITextObserver* observer );
    virtual void last_write_time_changed( const boost::filesystem::path& file );
    virtual void disabled( size_t key );

private:

    bool reload();
    size_t hash( std::wstring s );
    void notify();

private:

    std::set<size_t> m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    boost::filesystem::path m_file_path;
    std::wstring m_string;
    boost::hash<std::wstring> m_hash;
    std::set<ITextObserver*> m_observers;
};
