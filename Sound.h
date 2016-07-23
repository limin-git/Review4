#pragma once
#include "ISound.h"


struct Sound : ISound
{
public:

    Sound();
    ~Sound();
    virtual bool play_sound( const boost::filesystem::path& sound_file );
    virtual bool play_sound( const std::vector<boost::filesystem::path>& sound_files );
};
