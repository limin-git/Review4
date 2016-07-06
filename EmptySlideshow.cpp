#include "stdafx.h"
#include "EmptySlideshow.h"
#include "IConsole.h"


EmptySlideshow::EmptySlideshow( const std::wstring& message )
    : m_message( message )
{
}


bool EmptySlideshow::show()
{
    IConsole::instance()
        .cls()
        .write_center( m_message );
    return true;
}


size_t EmptySlideshow::key()
{
    return 0;
}


const std::wstring& EmptySlideshow::key_string()
{
    return m_message;
}
