#include "stdafx.h"
#include "ITextFactory.h"
#include "TextFactory.h"


static ITextFactory* g_text_factory = NULL;


ITextFactory& ITextFactory::instance()
{
    if ( g_text_factory == NULL )
    {
        g_text_factory = new TextFactory;
    }

    return *g_text_factory;
}


void ITextFactory::remove()
{
    delete g_text_factory;
    g_text_factory = NULL;
}
