#pragma once
#include "IReview.h"
#include "ISlideShow.h"
#include "IConfigurationFileObserver.h"


struct TextReview : IReview,
                    IConfigurationFileObserver
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
    virtual void handle_review_again();
    virtual void handle_listen();
    virtual void handle_filter();
    virtual void options_changed( const po::variables_map& vm, const po::variables_map& old );

protected:

    void show();
    void go_forward();
    void go_back();
    void delete_review_history( size_t key );
    void listen_thread_function();

protected:

    ISlideshowPtrList m_review_history;
    std::list< std::pair<int, ISlideshowPtr> > m_review_again;
    std::map<size_t, size_t> m_review_times;
    std::set<size_t> m_review_again_set;
    ISlideshowPtrList::iterator m_current;
    bool m_current_show_finished;
    int m_index;
    int m_review_again_distance;
    int m_listen_interval;
    volatile bool m_listening;
    volatile size_t m_auto_review_again;
};
