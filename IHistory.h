#pragma once


struct IHistory
{
    virtual ~IHistory() {}
    virtual const std::vector<std::time_t>& history( size_t key ) = 0;
    virtual void write_history( size_t key, std::time_t = ::time( NULL ) ) = 0;
    virtual void save_history_file() = 0;
    static IHistory& instance();
    static void remove();
};
