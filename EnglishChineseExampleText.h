#pragma once
#include "IText.h"
#include "IFileChangeHandler.h"


struct EnglishChineseExampleText : IText,
                                   IFileChangeHandler
{
public:

    EnglishChineseExampleText( const boost::filesystem::path& file_path );
    virtual ISlideshowPtr slideshow( size_t key );
    virtual const std::set<size_t>& keys();
    virtual void last_write_time_changed( const boost::filesystem::path& file );

private:

    void reload();
    size_t hash( std::wstring s );

private:
    
    std::set<size_t> m_keys;
    std::map<size_t, ISlideshowPtr> m_slidshow_map;
    boost::filesystem::path m_file_path;
    std::wstring m_string;
    boost::hash<std::wstring> m_hash;
};
