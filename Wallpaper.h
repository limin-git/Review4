#pragma once
#include "IWallpaper.h"
#include "IInputHandler.h"
#include "IInput.h"
#include "IConfigurationFile.h"
#include "Singleton.h"
#include "IReview.h"

namespace fs = boost::filesystem;


struct Wallpaper : IReview,
                   IInputHandler
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

protected:

    void remove_current_picture();

private:

    void set_wallpaper();

private:

    fs::path m_directory;
    fs::path m_recycle_directory;
    std::list<fs::path> m_pictures;
    std::list<fs::path>::iterator m_current;
    Singleton<IConfigurationFile> m_configuration;
    Singleton<IInput> m_input;
};
