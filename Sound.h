#pragma once
#include "ISound.h"


struct Sound : ISound
{
public:

    Sound();
    ~Sound();
    virtual void play_sound( const boost::filesystem::path& sound_file );
    virtual void play_sound( const std::vector<boost::filesystem::path>& sound_files );
};
