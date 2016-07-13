#include "stdafx.h"
#include "IReviewManager.h"
#include "ReviewManager.h"


IReviewManager& IReviewManager::instance()
{
    static ReviewManager review_manager;
    return review_manager;
}
