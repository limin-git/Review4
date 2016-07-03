#include "stdafx.h"
#include "TextFactory.h"
#include "EnglishChineseExampleText.h"


TextFactory::TextFactory()
{
}


TextFactory::~TextFactory()
{
}


IText* TextFactory::create_text( const boost::filesystem::path& file_name )
{
    if ( file_name.extension() == ".ece" )
    {
        return new EnglishChineseExampleText( file_name );
    }

    return NULL;
}
