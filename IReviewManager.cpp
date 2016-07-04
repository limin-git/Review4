#include "stdafx.h"
#include "IReviewManager.h"
#include "ReviewManager.h"


static IReviewManager* s_review_manager = NULL;


IReviewManager& IReviewManager::instance()
{
    if ( s_review_manager == NULL )
    {
        s_review_manager = new ReviewManager;
    }

    return *s_review_manager;
}


void IReviewManager::remove()
{
    delete s_review_manager;
    s_review_manager = NULL;
}
