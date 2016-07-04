#pragma once
#include "TimeUtility.h"
#include "StringUtility.h"
#include "UnicodeUtility.h"
#include "SoundUtility.h"
#include "ConsoleUtility.h"
#include "FileUtility.h"
#include "WriteConsoleHelper.h"

#ifdef max
#undef max
#undef min
#endif


namespace Utility
{
    size_t random( size_t lo, size_t hi );
    void set_system_wallpaper( const std::wstring& picture );
    std::vector<std::wstring> get_files_of_directory( const std::wstring& dir );
    bool is_picture( const std::wstring& file_name );
    bool remove_file( const std::wstring& file_name );
}
