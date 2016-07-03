#pragma once
#include "ITextFactory.h"


struct TextFactory : ITextFactory
{
public:

    TextFactory();
    ~TextFactory();
    virtual IText* create_text( const boost::filesystem::path& file_name );
};
