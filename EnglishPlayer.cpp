#include "stdafx.h"
#include "EnglishPlayer.h"
#include "IConfigurationFile.h"
#include "ISpeech.h"
#include "ISound.h"
#include "Utility.h"

#define speech_path         "speech.path"
#define speech_disable      "speech.disable"
#define speech_synchronized "speech.synchronized"


EnglishPlayer::EnglishPlayer()
    : m_options_description( "Speech" ),
      m_disabled( false ),
      m_is_synchronized( false )
{
    m_options_description.add_options()
        ( speech_path,          po::wvalue< std::vector<std::wstring> >(),      "speech path with format 'path | .extension'" )
        ( speech_disable,       po::wvalue<std::wstring>(),                     "disable speech (true/false)" )
        ( speech_synchronized,  po::wvalue<std::wstring>(),                     "synchronized or asynchronized" )
        ;
    IConfigurationFile::instance().add_options_description( m_options_description ).add_observer( this );
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

    if ( m_is_synchronized )
    {
        speak_impl( w );
    }
    else
    {
        m_processor.queue_item( boost::bind( &EnglishPlayer::speak_impl, this, w ) );
    }
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
    static ISpeech& speech = ISpeech::instance();
    static ISound& sound = ISound::instance();

    if ( word.path.empty() )
    {
        speech.speak( word.word );
    }
    else
    {
        if ( ! sound.play_sound( word.path ) )
        {
            speech.speak( word.word );
        }
    }
}


bool EnglishPlayer::is_synchronized()
{
    return m_is_synchronized;
}


void EnglishPlayer::synchronize( bool is_sync )
{
    m_is_synchronized = is_sync;
}


void EnglishPlayer::options_changed( const po::variables_map& vm, const po::variables_map& old )
{
    if ( Utility::updated< std::vector<std::wstring> >( speech_path, vm, old ) )
    {
        SpeechDirectory speech_directories;
        std::vector<std::wstring> dirs = vm[speech_path].as< std::vector<std::wstring> >();

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

    if ( Utility::updated<std::wstring>( speech_disable, vm, old ) )
    {
        const std::wstring s = vm[speech_disable].as<std::wstring>();
        m_disabled = ( L"true" == s || L"1" == s || L"TRUE" == s || L"True" == s );
    }

    if ( Utility::updated<std::wstring>( speech_synchronized, vm, old ) )
    {
        m_is_synchronized = ( L"true" == vm[speech_synchronized].as<std::wstring>() );
    }
}
