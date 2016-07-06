#include "stdafx.h"
#include "Scheduler.h"
#include "IConfigurationFile.h"
#include "TimeUtility.h"
#include "StringUtility.h"
#include "IHistory.h"
#include "IText.h"
#include "Utility.h"
#include "EmptySlideshow.h"
#include "IDisable.h"
#include "IText.h"
#include "IConsole.h"

// dependency:
// IHistory
//     --> IConfigurationFile
//     --> IDisable
//     --> IText
//     --> IHistory
//     --> IConsole

namespace po = boost::program_options;
static const std::time_t ONE_DAY = 12 * 3600;


Scheduler::Scheduler()
    : m_running( true ),
      m_select_candidates_semaphore( 0 ),
      m_select_candidates_thread( NULL ),
      m_once_per_session( false )
{
    //TODO: lock when keys wer pressed down for a long time.
    po::options_description options( "Scheduler" );
    options.add_options()
        ( "review.schedule", po::wvalue<std::wstring>(), "schedule" )
        ( "review.once-per-session", po::wvalue<std::wstring>(), "once per session" )
        ;
    po::variables_map& vm = IConfigurationFile::instance().add_options_description( options ).variables_map();
    m_once_per_session = ( L"true" == vm["review.once-per-session"].as<std::wstring>() );
    std::wstring schedule_string;

    if ( vm.count( "review.schedule" ) )
    {
        schedule_string = vm["review.schedule"].as<std::wstring>();
    }

    if ( schedule_string.empty() )
    {
        schedule_string = L"7 : 24 hours, 48 hours, 72 hours, 96 hours, 120 hours, 144 hours, 168 hours";
    }

    size_t pos = schedule_string.find_first_of( L":|" );

    if ( pos == std::wstring::npos )
    {
        return;
    }

    size_t number = boost::lexical_cast<size_t>( boost::trim_copy( schedule_string.substr( 0, pos ) ) );
    std::vector<std::time_t> schedule = Utility::times_from_strings( Utility::split_string( schedule_string.substr( pos + 1 ) ) );
    number = ( 0 == number ? 100 : number );

    if ( 1 == schedule.size() )
    {
        m_schedule.reserve( number );

        for ( size_t i = 0; i < number; ++i )
        {
            m_schedule.push_back( schedule[0] );
        }
    }
    else
    {
        m_schedule = schedule;
    }

    initialize_schedule();

    m_select_candidates_thread = new boost::thread( boost::bind( &Scheduler::select_candidates_thread, this ) );
    IDisable::instance().add_observer( this );
    IText::instance().add_observer( this );
}


Scheduler::~Scheduler()
{
    m_running = false;
    m_select_candidates_semaphore.post();
    m_select_candidates_thread->join();
    delete m_select_candidates_thread;
    IDisable::instance().remove_observer( this );
    IText::instance().remove_observer( this );
}


ISlideshowPtr Scheduler::get_slideshow()
{
    if ( m_schedule.empty() )
    {
        return ISlideshowPtr( new EmptySlideshow( L"wrong schedule, check configuration!!!" ) );
    }

    if ( m_candidates.empty() )
    {
        m_select_candidates_semaphore.post();
        return ISlideshowPtr( new EmptySlideshow( m_next_time_map.empty() ? L"finished." : L"empty." ) );
    }

    std::set<size_t>::iterator it = m_candidates.begin();

    if ( 1 < m_candidates.size() )
    {
        size_t random = Utility::random( 0, m_candidates.size() - 1 );
        std::advance( it, random );
    }

    size_t key = *it;
    m_candidates.erase( it );
    IHistory::instance().write_history( key );
    ISlideshowPtr slideshow = IText::instance().slideshow( key );

    if ( is_finished( key ) )
    {
        IDisable::instance().disable( slideshow ); // same as disabled
    }
    else
    {
        if ( ! m_once_per_session )
        {
            schedule_next_time( key );
        }
    }

    set_title();
    m_select_candidates_semaphore.post();
    return slideshow;
}


void Scheduler::initialize_schedule()
{
    std::time_t current = std::time( NULL );
    const std::set<size_t>& keys = IText::instance().keys();
    std::set<size_t> candidates;
    std::multimap<std::time_t, size_t> next_time_map;

    BOOST_FOREACH( size_t key, keys )
    {
        std::time_t next_time = get_next_time( key, current );

        if ( next_time <= current )
        {
            candidates.insert( key );
        }
        else if ( next_time - current < ONE_DAY )
        {
            next_time_map.insert( std::make_pair( next_time , key ) );
        }
    }

    m_candidates.swap( candidates );
    m_next_time_map.swap( next_time_map );
}


void Scheduler::select_candidates_thread()
{
    while ( m_running )
    {
        // TODO: timed wait
        m_select_candidates_semaphore.wait();

        if ( !m_running )
        {
            return;
        }

        std::set<size_t> candidates;
        std::time_t current = std::time( NULL );

        for ( std::multimap<std::time_t, size_t>::iterator it = m_next_time_map.begin(); it != m_next_time_map.end(); )
        {
            if ( it->first <= current )
            {
                candidates.insert( it->second );
                m_next_time_map.erase( it++ );
            }
            else
            {
                break;
            }
        }

        if ( !candidates.empty() )
        {
            m_candidates.insert( candidates.begin(), candidates.end() );
            set_title();
        }
    }
}


void Scheduler::schedule_next_time( size_t key )
{
    std::time_t current = std::time( NULL );
    std::time_t next_time = get_next_time( key, current );

    if ( next_time - current < ONE_DAY )
    {
        m_next_time_map.insert( std::make_pair( next_time , key ) );
    }
}


std::time_t Scheduler::get_next_time( size_t key, const std::time_t current )
{
    const std::vector<std::time_t>& history = IHistory::instance().history( key );
    return ( history.empty() ? current : history.back() + m_schedule[ history.size() - 1 ] );
}


bool Scheduler::is_finished( size_t key )
{
    return ( m_schedule.size() + 1 <= IHistory::instance().history( key ).size() );
}


void Scheduler::disabled( size_t key )
{
    m_candidates.erase( key );

    for ( std::multimap<std::time_t, size_t>::iterator it = m_next_time_map.begin(); it != m_next_time_map.end(); )
    {
        if ( it->second == key )
        {
            m_next_time_map.erase( it++ );
        }
        else
        {
            ++it;
        }
    }
}


void Scheduler::text_changed( IText* text )
{
    initialize_schedule();
}


void Scheduler::set_title()
{
    std::wstringstream strm;
    strm << IText::instance().get_file_path().filename().wstring() << " - " << m_candidates.size();
    IConsole::instance().title( strm.str() );
}
