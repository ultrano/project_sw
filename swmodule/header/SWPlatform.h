#ifndef SWPlatform_h__
#define SWPlatform_h__

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifdef WIN32

# define PLATFORM_WIN32

#elif TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE

# define PLATFORM_IOS

#elif TARGET_OS_MAC

# define PLATFORM_OSX

#elif ANDROID

# define PLATFORM_ANDROID

#endif


#endif //! SWPlatform_h__