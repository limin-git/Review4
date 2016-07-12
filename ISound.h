#pragma once


struct ISound
{
    virtual ~ISound() {};
    virtual void play_sound( const boost::filesystem::path& sound_file ) = 0;
    virtual void play_sound( const std::vector<boost::filesystem::path>& sound_files ) = 0;
    static ISound& instance();
};
