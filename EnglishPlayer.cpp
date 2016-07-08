#include "stdafx.h"
#include "EnglishPlayer.h"
#include "IConfigurationFile.h"
#include "ISpeech.h"
#include "ISound.h"
#include "Utility.h"


EnglishPlayer::EnglishPlayer()
    : m_options_description( "Speech" ),
      m_speech( ISpeech::instance() ),
      m_sound( ISound::instance() ),
      m_disabled( false )
{
    m_processor.set_callback( boost::bind( &EnglishPlayer::speak_impl, this, _1 ) );

    m_options_description.add_options()
        ( "speech.path", po::wvalue< std::vector<std::wstring> >(), "speech path with format 'path | .extension'" )
        ( "speech.disable", po::wvalue<std::wstring>(), "disable speech (true/false)" )
        ;

    IConfigurationFile::instance()
        .add_options_description( m_options_description )
        .add_observer( this)
        ;
}


EnglishPlayer::~EnglishPlayer()
{
    m_processor.terminate();
    IConfigurationFile::instance().remove_observer( this );
}


void EnglishPlayer::speak( const std::wstring& word )
{
    if ( m_disabled || word.empty() )
    {
        return;
    }

    Word w;
    w.word = word;
    fs::path initial = word.substr( 0, 1 );

    BOOST_FOREACH( SpeechDirectory::value_type& dir, m_speech_directories )
    {
        fs::path p = dir.first / initial / ( word + dir.second );

        if ( exists( p ) )
        {
            w.path = p;
            break;
        }
    }

    m_processor.queue_item( w );
}


void EnglishPlayer::speak( const std::vector<std::wstring>& words )
{
    BOOST_FOREACH( const std::wstring& word, words )
    {
        speak( word );
    }
}


void EnglishPlayer::speak_impl( const Word& word )
{
    if ( word.path.empty() )
    {
        m_speech.speak( word.word );
    }
    else
    {
        m_sound.play_sound( word.path );
    }
}


void EnglishPlayer::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated< std::vector<std::wstring> >( "speech.path", vm, old ) )
    {
        SpeechDirectory speech_directories;
        std::vector<std::wstring> dirs = vm["speech.path"].as< std::vector<std::wstring> >();

        for ( size_t i = 0; i < dirs.size(); ++i )
        {
            size_t pos = dirs[i].find( L"|" );

            if ( pos != std::wstring::npos )
            {
                fs::path dr = boost::trim_copy( dirs[i].substr( 0, pos ) );

                if ( exists( dr ) && is_directory( dr ) )
                {
                    speech_directories[dr] = boost::trim_copy( dirs[i].substr( pos + 1 ) );;
                }
            }
        }

        m_speech_directories.swap( speech_directories );
    }

    if ( Utility::updated<std::wstring>( "speech.disable", vm, old ) )
    {
        const std::wstring s = vm["speech.disable"].as<std::wstring>();
        m_disabled = ( L"true" == s || L"1" == s || L"TRUE" == s || L"True" == s );
    }
}
