#pragma once


struct IReviewManager
{
    virtual ~IReviewManager() {}
    virtual void run() = 0;
    static IReviewManager& instance();
    static void remove();
};
