#include "stdafx.h"
#include "Schedule.h"
#include "IConfigurationFile.h"
#include "TimeUtility.h"
#include "StringUtility.h"
#include "IHistory.h"
#include "IText.h"

namespace po = boost::program_options;


Schedule::Schedule()
{
    po::options_description desc( "Schedule" );
    desc.add_options()
        ( "review.schedule", po::wvalue<std::wstring>(), "schedule" )
        ;
    IConfigurationFile::instance().add_options_description( desc );

    std::wstring schedule = IConfigurationFile::instance().variables_map()["review.schedule"].as<std::wstring>();
    size_t pos = schedule.find_first_of( L":|" );

    if ( pos == std::wstring::npos )
    {
        return;
    }

    size_t number = boost::lexical_cast<size_t>( schedule.substr( 0, pos ).c_str() );
    std::vector<std::wstring> time_strings = Utility::split_string( schedule.substr( pos + 1 ) );
    std::vector<std::time_t> times = Utility::times_from_strings( time_strings );
    number = ( 0 == number ? 100 : number );

    if ( 1 == times.size() )
    {
        m_schedule.reserve( number );

        for ( size_t i = 0; i < number; ++i )
        {
            m_schedule.push_back( ( i + 1 ) * times[0] );
        }
    }
    else
    {
        m_schedule = times;
    }    
}


Schedule::~Schedule()
{
}


ISlideshowPtr Schedule::next_slideshow()
{
    return ISlideshowPtr();
}


bool Schedule::is_expired( size_t key, std::time_t current )
{
    std::vector<std::time_t> history = IHistory::instance().history( key );

    if ( history.empty() )
    {
        return true;
    }

    if ( m_schedule.size() + 1 <= history.size() ) // finished
    {
        return false;
    }

    size_t round = history.size();
    std::time_t last_time = history.back();
    std::time_t schedule_time = last_time + m_schedule[ round - 1 ];
    return ( current < schedule_time );
}


void Schedule::caclute_schedule_time()
{
    const std::set<size_t>& keys = IText::instance().keys();
    std::time_t current = std::time( NULL );

    BOOST_FOREACH( size_t key, keys )
    {
        const std::vector<std::time_t>& history = IHistory::instance().history( key );
        size_t round = history.size();

        if ( round == 0 )
        {
        }

        std::time_t last_time = history.back();
        std::time_t schedule_time = last_time + m_schedule[ round - 1 ];
    }
}
