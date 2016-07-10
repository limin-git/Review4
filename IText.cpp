#include "stdafx.h"
#include "IText.h"
#include "ITextFactory.h"
#include "IConfigurationFile.h"
#include "Singleton.h"


static IText* s_text = NULL;


IText& IText::instance()
{
    if ( s_text == NULL )
    {
        po::options_description options;
        options.add_options()
            ( "file.name", po::wvalue<std::wstring>(), "file name" )
            ;
        s_text = Singleton<ITextFactory>()->create_text( Singleton<IConfigurationFile>()
                                                           ->add_options_description( options )
                                                           .variables_map()["file.name"].as<std::wstring>() );
    }

    return *s_text;
}


void IText::remove()
{
    delete s_text;
    s_text = NULL;
}
