#include "stdafx.h"
#include "TestSound.h"
#include "ISound.h"


TestSound::TestSound()
{
    //test1();
    test2();
}


void TestSound::test1()
{
    while ( true )
    {
        ISound::instance().play_sound( "C:\\Windows\\Media\\Windows Error.wav" );

        if ( _getch() == 'q' )
        {
            break;
        }
    }
}

void TestSound::test2()
{
    //conclusion: can not do it like this.
    //            should always create and release. 

    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );

    IGraphBuilder* graph = NULL;
    IMediaControl* control = NULL;
    IMediaEvent*   evnt = NULL;

    ::CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&graph );
    graph->QueryInterface( IID_IMediaControl, (void **)&control );
    graph->QueryInterface( IID_IMediaEvent, (void **)&evnt );

    while ( true )
    {
        try
        {
            HRESULT hr = graph->RenderFile( L"C:\\Windows\\Media\\Windows Error.wav", NULL );

            if ( SUCCEEDED(hr) )
            {
                hr = control->Run();

                if ( SUCCEEDED(hr) )
                {
                    long code;
                    evnt->WaitForCompletion( INFINITE, &code );
                }
            }
        }
        catch ( ... )
        {
        }

        if ( _getch() == 'q' )
        {
            break;
        }
    }

    evnt->Release();
    control->Release();
    graph->Release();

    ::CoUninitialize();
}
