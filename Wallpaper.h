#pragma once
#include "IWallpaper.h"
#include "IInputHandler.h"
#include "IReview.h"
#include "IConfigurationFileObserver.h"
namespace fs = boost::filesystem;


struct Wallpaper : IReview,
                   IInputHandler,
                   IConfigurationFileObserver
{
public:

    Wallpaper();
    ~Wallpaper();

    virtual void handle_start();
    virtual void handle_continue();
    virtual void handle_replay();
    virtual void handle_next();
    virtual void handle_previous();
    virtual void handle_jump( size_t distance );
    virtual void handle_jump_back( size_t distance );
    virtual void handle_disable();
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

protected:

    void remove_current_picture();

private:

    void set_wallpaper();
    void search_pictures_thread();

private:

    fs::path m_directory;
    fs::path m_recycle_directory;
    std::list<fs::path> m_pictures;
    std::list<fs::path>::iterator m_current;
    size_t m_frequence;
    size_t m_count;
    bool m_disable;
};
