#include "stdafx.h"
#include "IText.h"
#include "ITextFactory.h"
#include "IConfigurationFile.h"


static IText* s_text = NULL;


IText& IText::instance()
{
    if ( s_text == NULL )
    {
        po::options_description desc;
        desc.add_options()
            ( "file.name", po::wvalue<std::wstring>(), "file name" )
            ;
        boost::filesystem::path file_name =
        IConfigurationFile::instance()
            .add_options_description( desc )
            .variables_map()["file.name"].as<std::wstring>();
        s_text = ITextFactory::instance().create_text( file_name );
    }

    return *s_text;
}


void IText::remove()
{
    delete s_text;
    s_text = NULL;
}
