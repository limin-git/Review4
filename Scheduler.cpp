#include "stdafx.h"
#include "Scheduler.h"
#include "IConfigurationFile.h"
#include "TimeUtility.h"
#include "StringUtility.h"
#include "IHistory.h"
#include "IText.h"
#include "AlgorithmUtility.h"
#include "Utility.h"
#include "EmptySlideshow.h"
#include "IDisable.h"
#include "IText.h"
#include "IConsole.h"
namespace po = boost::program_options;
static const std::time_t ONE_DAY = 12 * 3600;

#define review_schedule             "review.schedule"
#define review_once_per_session     "review.once-per-session"
#define review_randomize            "review.randomize"


Scheduler::Scheduler()
    : m_running( true ),
      m_select_candidates_semaphore( 0 ),
      m_once_per_session( false ),
      m_randomize( false ),
      m_initialized( false )
{
    //TODO: lock when keys wer pressed down for a long time.
    po::options_description options( "Scheduler" );
    options.add_options()
        ( review_schedule,          po::wvalue<std::wstring>(), "schedule" )
        ( review_once_per_session,  po::wvalue<std::wstring>(), "once per session" )
        ( review_randomize,         po::wvalue<std::wstring>(), "randomize or not" )
        ;
    IConfigurationFile::instance().add_options_description( options ).add_observer( this );
    parse_schedule_configuration();

    if ( ! m_schedule.empty() )
    {
        m_select_candidates_thread = boost::thread( boost::bind( &Scheduler::select_candidates_thread, this ) );
    }

    IDisable::instance().add_observer( this );
    IText::instance().add_observer( this );
}


Scheduler::~Scheduler()
{
    m_running = false;
    m_select_candidates_semaphore.post();
    m_select_candidates_thread.join();
    //IDisable::instance().remove_observer( this );
    //IText::instance().remove_observer( this );
}


ISlideshowPtr Scheduler::get_slideshow()
{
    boost::lock_guard<boost::recursive_mutex> lock( m_mutex );

    if ( ! m_initialized )
    {
        initialize_schedule();
    }

    if ( m_candidates.empty() )
    {
        m_select_candidates_semaphore.post();
        return ISlideshowPtr( new EmptySlideshow( m_next_time_map.empty() ? L"finished." : L"empty." ) );
    }

    size_t key = m_candidates.front();
    m_candidates.pop_front();

    if ( m_candidates.empty() )
    {
        m_select_candidates_semaphore.post();
    }

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
    return slideshow;
}


void Scheduler::initialize_schedule()
{
    m_initialized = true;

    if ( m_schedule.empty() )
    {
        m_candidates = IText::instance().keys();
        return;
    }

    std::time_t current = std::time( NULL );
    const KeyList& keys = IText::instance().keys();
    KeyList candidates;
    std::multimap<std::time_t, size_t> next_time_map;

    BOOST_FOREACH( size_t key, keys )
    {
        std::time_t next_time = get_next_time( key, current );
        if ( 0 == next_time )
        {
            IDisable::instance().disable( IText::instance().slideshow( key ) );
        }
        else if ( next_time <= current )
        {
            candidates.push_back( key );
        }
        else if ( next_time - current < ONE_DAY )
        {
            next_time_map.insert( std::make_pair( next_time , key ) );
        }
    }

    if ( m_randomize )
    {
        Utility::random_shuffle( candidates );
    }

    m_candidates.swap( candidates );
    m_next_time_map.swap( next_time_map );
}


void Scheduler::select_candidates_thread()
{
    while ( m_running )
    {
        m_select_candidates_semaphore.wait();

        if ( !m_running )
        {
            return;
        }

        KeyList candidates;
        std::time_t current = std::time( NULL );

        for ( std::multimap<std::time_t, size_t>::iterator it = m_next_time_map.begin(); it != m_next_time_map.end(); )
        {
            if ( it->first <= current )
            {
                candidates.push_back( it->second );
                m_next_time_map.erase( it++ );
            }
            else
            {
                break;
            }
        }

        if ( !candidates.empty() )
        {
            boost::lock_guard<boost::recursive_mutex> lock( m_mutex );
            m_candidates.insert( m_candidates.end(), candidates.begin(), candidates.end() );
            if ( m_randomize )
            {
                Utility::random_shuffle( m_candidates );
            }
            set_title();
        }
    }
}


void Scheduler::schedule_next_time( size_t key )
{
    if ( m_schedule.empty() )
    {
        return;
    }

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
    if ( m_schedule.size() + 1 <=  history.size() )
    {
        return 0;
    }
    return ( history.empty() ? current : history.back() + m_schedule[ history.size() - 1 ] );
}


bool Scheduler::is_finished( size_t key )
{
    if ( m_schedule.empty() )
    {
        return false;
    }

    return ( m_schedule.size() + 1 <= IHistory::instance().history( key ).size() );
}


void Scheduler::disabled( size_t key )
{
    m_candidates.remove( key );

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


void Scheduler::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated<std::wstring>( review_once_per_session, vm, old ) )
    {
        m_once_per_session = ( L"true" == vm[review_once_per_session].as<std::wstring>() );
    }

    if ( Utility::updated<std::wstring>( review_randomize, vm, old ) )
    {
        m_randomize = ( L"true" == vm[review_randomize].as<std::wstring>() );
    }
}


void Scheduler::parse_schedule_configuration()
{
    po::variables_map& vm = IConfigurationFile::instance().variables_map();
    std::wstring schedule_string;

    if ( vm.count( review_schedule ) )
    {
        schedule_string = vm[review_schedule].as<std::wstring>();
    }

    if ( schedule_string == L"disable" || schedule_string == L"no" )
    {
        return;
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
}
