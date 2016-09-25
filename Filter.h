#pragma once
#include "IFilter.h"
namespace fs = boost::filesystem;


struct Filter : IFilter
{
public:

    Filter();
    ~Filter();
    virtual void filter( ISlideshowPtr slideshow );
    virtual bool is_filtered( const std::wstring& str );

private:

    void load_file();
    void notify( size_t key );

private:

    fs::path m_file_name;
    std::wfstream m_file_stream;
    std::set<std::wstring> m_filter;
};
