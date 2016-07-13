#include "stdafx.h"
#include "TextFactory.h"
#include "EnglishChineseExampleText.h"
#include "SrtSubtitleText.h"
#include "AssSubtitleText.h"


IText& TextFactory::create_text( const boost::filesystem::path& file_name )
{
    if ( file_name.extension() == ".ece" )
    {
        static EnglishChineseExampleText text( file_name );
        return text;
    }
    else if ( file_name.extension() == ".srt" )
    {
        static SrtSubtitleText text( file_name );
        return text;
    }
    else if ( file_name.extension() == ".ass" )
    {
        static AssSubtitleText text( file_name );
        return text;
    }

    IText* ptext = NULL;
    return *ptext;
}
