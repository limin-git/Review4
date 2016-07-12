#include "stdafx.h"
#include "ITextFactory.h"
#include "TextFactory.h"


ITextFactory& ITextFactory::instance()
{
    static TextFactory text_factory;
    return text_factory''
}
