#include "stdafx.h"
#include "IText.h"
#include "ITextFactory.h"
#include "IConfigurationFile.h"


static IText* s_text = NULL;


IText& IText::instance()
{
    if ( s_text == NULL )
    {
        boost::program_options::variables_map& vm = IConfigurationFile::instance().variables_map();
        s_text = ITextFactory::instance().create_text( vm["file.name"].as<std::wstring>() );
    }

    return *s_text;
}


void IText::remove()
{
    delete s_text;
    s_text = NULL;
}
