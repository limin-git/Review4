#pragma once


struct ISound
{
    virtual ~ISound() {};
    virtual bool play_sound( const boost::filesystem::path& sound_file ) = 0;
    virtual bool play_sound( const std::vector<boost::filesystem::path>& sound_files ) = 0;
    static ISound& instance();
};
