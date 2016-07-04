#pragma once
#include "IDisable.h"

namespace fs = boost::filesystem;


struct Disable : IDisable
{
public:

    Disable();
    ~Disable();
    virtual void disable( ISlideshowPtr slideshow );
    virtual const std::set<size_t>& disabled();

private:

    void load_file();
    void notify( size_t key );

private:

    fs::path m_file_name;
    std::set<size_t> m_disabled;
};
