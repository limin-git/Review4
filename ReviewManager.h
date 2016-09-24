#pragma once
#include "IReviewManager.h"
#include "IInputHandler.h"
#include "IHotKeyHandler.h"
#include "IReview.h"
#include "QueueProcessor.h"


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
    virtual void handle_review_again();
    virtual void handle_listen();

private:

    void start();
    void handle_exit();

private:

    bool m_register_hot_keys;
    typedef std::map<IReview*, IQueueProcessor*> ReviewMap;
    ReviewMap m_reivews;
};
