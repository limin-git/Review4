#include "stdafx.h"
#include "Schedule.h"
#include "IConfigurationFile.h"

namespace po = boost::program_options;


Schedule::Schedule()
{
    po::options_description desc( "Schedule" );
    desc.add_options()
        ( "review.schedule", po::value<std::string>(), "schedule" )
        ;
    IConfigurationFile::instance().add_options_description( desc );

    std::string schedule = IConfigurationFile::instance().variables_map()["review.schedule"].as<std::string>();

    std::stringstream strm( schedule );
    int number = 0;
    std::string sep; // separator
    strm >> number >> sep;


}


Schedule::~Schedule()
{

}


ISlideshowPtr Schedule::next_slideshow()
{
    return ISlideshowPtr();
}
