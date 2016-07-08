#pragma once
#include "IHistory.h"
#include "IDisableObserver.h"

namespace fs = boost::filesystem;


struct History : IHistory,
                 IDisableObserver
{
public:

    History();
    ~History();
    virtual const std::vector<std::time_t>& history( size_t key );
    virtual void write_history( size_t key, std::time_t t = ::time( NULL ) );
    virtual void disabled( size_t key );
    virtual void save_history_file();

private:

    void load_history();

private:

    typedef std::map< size_t, std::vector<std::time_t> > HistoryRecord;
    HistoryRecord m_history;
    fs::path m_file_name;
    std::fstream m_file_stream;
};
