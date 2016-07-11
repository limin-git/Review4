#pragma once
#include "IReviewManager.h"
#include "IInputHandler.h"
#include "ISlideShow.h"
#include "IHotKeyHandler.h"
#include "IDisable.h"
#include "IScheduler.h"
#include "IConfigurationFile.h"
#include "IInput.h"
#include "IHotKey.h"
#include "Singleton.h"
#include "IReview.h"
#include "ThreadPool.h"


struct ReviewManager : IReviewManager,
                       IInputHandler,
                       IHotKeyHandler
{
public:

    ReviewManager();
    ~ReviewManager();

    virtual void run();
    virtual void handle_continue();
    virtual void handle_replay();
    virtual void handle_next();
    virtual void handle_previous();
    virtual void handle_jump( size_t distance );
    virtual void handle_jump_back( size_t distance );
    virtual void handle_disable();

private:

    void start();
    void handle_impl_thread( IReview* review, void (IReview::*virtual_fun)() );
    void handle_impl_thread_1( IReview* review, void (IReview::*virtual_fun)(size_t), size_t distance );

private:

    bool m_register_hot_keys;
    Singleton<IConfigurationFile> m_configuration;
    Singleton<IInput> m_input;
    Singleton<IHotKey> m_hotkey;
    std::set<IReview*> m_reivews;
    ThreadPool m_thread_pool;
};
