#pragma once
#include "TextReview.h"


struct SubtitleReview : TextReview
{
public:

    virtual void handle_jump( size_t distance );
    virtual void handle_jump_back( size_t distance );
};
