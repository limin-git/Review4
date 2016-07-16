#include "stdafx.h"
#include "SubtitleReview.h"


void SubtitleReview::handle_jump( size_t distance )
{
    Sleep( 200 );

    for ( size_t i = 0; i < distance + 1; ++i )
    {
        go_forward();
    }

    m_current_show_finished = false;
    show();
}


void SubtitleReview::handle_jump_back( size_t distance )
{
    Sleep( 200 );

    for ( size_t i = 0; i < distance + 1; ++i )
    {
        go_back();
    }

    m_current_show_finished = false;
    show();
}
