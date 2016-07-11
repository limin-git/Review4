#pragma once
#include "IReview.h"
#include "IInputHandler.h"
#include "ISlideShow.h"
#include "IHotKeyHandler.h"
#include "IDisable.h"
#include "IScheduler.h"
#include "IConfigurationFile.h"
#include "IInput.h"
#include "IHotKey.h"
#include "Singleton.h"


struct TextReview : IReview
{
public:

    TextReview();
    ~TextReview();

    virtual void handle_start();
    virtual void handle_continue();
    virtual void handle_replay();
    virtual void handle_next();
    virtual void handle_previous();
    virtual void handle_jump( size_t distance );
    virtual void handle_jump_back( size_t distance );
    virtual void handle_disable();

protected:

    void show();
    void go_forward();
    void go_back();
    void delete_review_history( size_t key );

protected:

    ISlideshowPtrList m_review_history;
    ISlideshowPtrList::iterator m_current;
    bool m_current_show_finished;
    Singleton<IDisable> m_disable;
    Singleton<IScheduler> m_scheduler;
    Singleton<IConfigurationFile> m_configuration;
    Singleton<IInput> m_input;
    Singleton<IHotKey> m_hotkey;
};
