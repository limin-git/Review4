#pragma once
#include "IReview.h"
#include "ISlideShow.h"


struct TextReview : IReview
{
public:

    TextReview();
    ~TextReview();

    virtual void handle_start();
    virtual void handle_exit();
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
    boost::recursive_mutex m_mutex;
};
