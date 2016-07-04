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

private:

    void continue_handler();
    void next_handler();
    void back_handler();

private:

    ISlideshowPtr m_slideshow;
    ISlideshowPtrList m_slideshow_history;
};
