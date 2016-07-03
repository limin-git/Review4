#pragma once
#include "IText.h"


struct ITextFactory
{
    virtual ~ITextFactory() {}
    virtual IText* create_text( const boost::filesystem::path& file_name ) = 0;
    static ITextFactory& instance();
    static void remove();
};
