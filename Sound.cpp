#include "stdafx.h"
#include "Sound.h"
#include <dshow.h>
#pragma comment(lib, "strmiids.lib") // For IID_IGraphBuilder, IID_IMediaControl, IID_IMediaEvent


Sound::Sound()
{
    ::CoInitializeEx( NULL, COINIT_MULTITHREADED );
}


Sound::~Sound()
{
    ::CoUninitialize();
}


bool Sound::play_sound( const boost::filesystem::path& sound_file )
{
    bool success = true;

    try
    {
        IGraphBuilder* graph = NULL;
        IMediaControl* control = NULL;
        IMediaEvent*   evnt = NULL;

        ::CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&graph );
        graph->QueryInterface( IID_IMediaControl, (void **)&control );
        graph->QueryInterface( IID_IMediaEvent, (void **)&evnt );

        HRESULT hr = graph->RenderFile( sound_file.wstring().c_str(), NULL );

        if ( SUCCEEDED(hr) )
        {
            hr = control->Run();

            if ( SUCCEEDED(hr) )
            {
                long code;
                evnt->WaitForCompletion( INFINITE, &code );
            }
        }
        else
        {
            //TODO: why can not open affair.wav?
            success = false;
        }

        evnt->Release();
        control->Release();
        graph->Release();
    }
    catch ( ... )
    {
    }

    return success;
}


bool Sound::play_sound( const std::vector<boost::filesystem::path>& sound_files )
{
    bool success = true;

    for ( size_t i = 0; i < sound_files.size(); ++i )
    {
        success &= play_sound( sound_files[i] );
    }

    return success;
}
