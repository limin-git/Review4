#include "stdafx.h"
#include "AbstructText.h"
#include "EmptySlideshow.h"
#include "IDisable.h"


AbstructText::AbstructText( const fs::path& file_path )
    : m_file_path( file_path ),
      m_parsed( false )
{
    m_file_path = system_complete( m_file_path );
    IDisable::instance()->add_observer( this );
}


AbstructText::~AbstructText()
{
    IDisable::instance()->remove_observer( this );
}


ISlideshowPtr AbstructText::slideshow( size_t key )
{
    if ( ! m_parsed )
    {
        parse_text();
        m_parsed = true;
    }

    std::map<size_t, ISlideshowPtr>::iterator it = m_slidshow_map.find( key );

    if ( it == m_slidshow_map.end() )
    {
        return ISlideshowPtr( new EmptySlideshow );
    }

    return it->second;
}


const fs::path& AbstructText::get_file_path()
{
    return m_file_path;
}


const KeyList& AbstructText::keys()
{
    if ( ! m_parsed )
    {
        parse_text();
        m_parsed = true;
    }

    return m_keys;
}


void AbstructText::add_observer( ITextObserver* observer )
{
    m_observers.insert( observer );
}


void AbstructText::remove_observer( ITextObserver* observer )
{
    m_observers.erase( observer );
}


void AbstructText::disabled( size_t key )
{
    if ( ! m_parsed )
    {
        parse_text();
        m_parsed = true;
    }

    m_keys.remove( key );
    m_slidshow_map.erase( key );
}
