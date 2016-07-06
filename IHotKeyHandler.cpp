#include "stdafx.h"
#include "IHotKeyHandler.h"


static int s_hot_key_id = 1;


int IHotKeyHandler::unique_id()
{
    return s_hot_key_id++;
}
