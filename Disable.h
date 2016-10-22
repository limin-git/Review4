#pragma once
#include "IDisable.h"
namespace fs = boost::filesystem;


struct Disable : IDisable
{
public:

    Disable();
    ~Disable();
    virtual void disable( ISlideshowPtr slideshow );
    virtual const std::set<std::wstring>& disabled();
    virtual void add_observer( IDisableObserver* observer );
    virtual void remove_observer( IDisableObserver* observer );
    virtual bool is_disabled( size_t key );
    virtual bool is_disabled( const std::wstring& key );

private:

    void load_file();
    void notify( size_t key );

private:

    fs::path m_file_name;
    std::wfstream m_file_stream;
    std::set<std::wstring> m_disabled;
    std::set<IDisableObserver*> m_observers;
};
