#include "stdafx.h"
#include "EmptySlideshow.h"
#include "IConsole.h"


EmptySlideshow::EmptySlideshow()
{
}


bool EmptySlideshow::show()
{
    IConsole::instance()
        .cls()
        .write_center( "empty." );
    return true;
}


size_t EmptySlideshow::key()
{
    return 0;
}


const std::wstring& EmptySlideshow::key_string()
{
    static const std::wstring empty = L"empty.";
    return empty;
}
