#include "stdafx.h"
#include "EmptySlideshow.h"
#include "IConsole.h"


EmptySlideshow::EmptySlideshow( bool finished )
    : m_finished( finished )
{
}


bool EmptySlideshow::show()
{
    IConsole::instance()
        .cls()
        .write_center( m_finished ? "finished." : "empty." );
    return true;
}


size_t EmptySlideshow::key()
{
    return 0;
}


const std::wstring& EmptySlideshow::key_string()
{
    static const std::wstring empty = L"empty.";
    static const std::wstring finished = L"finished.";
    return m_finished ? finished : empty;
}
