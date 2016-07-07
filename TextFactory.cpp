#include "stdafx.h"
#include "TextFactory.h"
#include "EnglishChineseExampleText.h"
#include "SrtSubtitleText.h"
#include "AssSubtitleText.h"


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
    else if ( file_name.extension() == ".srt" )
    {
        return new SrtSubtitleText( file_name );
    }
    else if ( file_name.extension() == ".ass" )
    {
        return new AssSubtitleText( file_name );
    }

    return NULL;
}
