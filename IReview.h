#pragma once


struct IReview
{
    virtual ~IReview() {}
    virtual void handle_start() = 0;
    virtual void handle_exit() = 0;
    virtual void handle_continue() = 0;
    virtual void handle_next() = 0;
    virtual void handle_previous() = 0;
    virtual void handle_replay() = 0;
    virtual void handle_disable() = 0;
    virtual void handle_jump( size_t distance ) = 0;
    virtual void handle_jump_back( size_t distance ) = 0;
    virtual void handle_review_again() {};
    virtual void handle_listen() {};
    virtual void handle_filter() {};
};
