#include "stdafx.h"
#include "FileChangeManager.h"
#include "WriteConsoleHelper.h"
//#include "Log.h"


void FileChangeManager::add_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler )
{
    if ( !exists( file_path ) || is_directory( file_path ) )
    {
        return;
    }

    FileChangeMonitorThread t;
    t.initialize( file_path.is_complete() ? file_path : system_complete( file_path ), handler );

    BOOST_FOREACH( FileChangeMonitorThread& v, m_threads )
    {
        if ( v.merge_child( t ) )
        {
            return;
        }
    }

    for ( FileChangeMonitorThreadList::iterator it = m_threads.begin(); it != m_threads.end(); )
    {
        if ( t.merge_child( *it ) )
        {
            m_threads.erase( it++ );
        }
        else
        {
            ++it;
        }
    }

    m_threads.push_back( t );
    m_threads.back().start();
}


void FileChangeManager::remove_handler( const boost::filesystem::path& file_path, IFileChangeHandler* handler )
{
    for ( FileChangeMonitorThreadList::iterator it = m_threads.begin(); it != m_threads.end(); ++it )
    {
        if ( it->is_monitoring( file_path ) )
        {
            it->remove_handler( file_path, handler );
            
            if ( it->empty() )
            {
                m_threads.erase( it );
            }

            return;
        }
    }
}
