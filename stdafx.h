// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#pragma warning( disable: 4995 4819 4996 4275 4267 )

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <fstream>
#include <sstream>

#define BOOST_ALL_DYN_LINK
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/function.hpp>
#include <boost/function_equal.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/functional.hpp>
#include <boost/functional/hash.hpp>
#include <boost/timer/timer.hpp>
#include <boost/chrono/io/duration_io.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
