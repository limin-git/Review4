#include "stdafx.h"
#include "IText.h"
#include "ITextFactory.h"
#include "IConfigurationFile.h"


IText& IText::instance()
{
    static IText* ptext = NULL;

    if ( NULL == ptext )
    {
        po::options_description options;
        options.add_options()
            ( "file.name", po::wvalue<std::wstring>(), "file name" )
            ;
        static IText& text = ITextFactory::instance().create_text( IConfigurationFile::instance()
            .add_options_description( options )
            .variables_map()["file.name"].as<std::wstring>() );
        ptext = &text;
    }

    return *ptext;
}
