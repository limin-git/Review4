#pragma once
#include "IReviewManager.h"
#include "IInputHandler.h"
#include "ISlideShow.h"


struct ReviewManager : IReviewManager,
                       IInputHandler
{
public:

    ReviewManager();
    ~ReviewManager();
    virtual void run();

protected:

    // handlers
    void continue_handler();
    void next_handler();
    void back_handler();
    void disable_handler();

private:

    void show();

private:

    ISlideshowPtr m_slideshow;
    ISlideshowPtrList m_slideshow_history;
};
