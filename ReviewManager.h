#pragma once
#include "IReviewManager.h"
#include "IInputHandler.h"
#include "IHotKeyHandler.h"
#include "IReview.h"
#include "QueueProcessor.h"
#include "IConfigurationFileObserver.h"


struct ReviewManager : IReviewManager,
                       IInputHandler,
                       IHotKeyHandler,
                       IConfigurationFileObserver
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
    virtual void handle_review_again();
    virtual void handle_listen();
    virtual void handle_filter();
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

private:

    void start();
    void handle_exit();
    void regist_hot_keys();
    void unregister_hot_keys();

private:

    bool m_register_hot_keys;
    bool m_disable_hotkeys_when_listening;
    typedef std::map<IReview*, IQueueProcessor*> ReviewMap;
    ReviewMap m_reivews;
    bool m_listening;
};
