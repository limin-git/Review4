#pragma once
#include "IReviewManager.h"
#include "IInputHandler.h"
#include "ISlideShow.h"
#include "IHotKeyHandler.h"


struct ReviewManager : IReviewManager,
                       IInputHandler,
                       IHotKeyHandler
{
public:

    ReviewManager();
    ~ReviewManager();
    virtual void run();

protected:

    // handlers
    void continue_handler();
    void next_handler();
    void previous_handler();
    void repeat_handler();
    void disable_handler();

private:

    void show();
    void delete_review_history( size_t key );
    void forward();

private:

    //ISlideshowPtr m_slideshow;
    ISlideshowPtrList m_slideshow_history;
    ISlideshowPtrList::iterator m_current;
    bool m_forward_before_show;
};
