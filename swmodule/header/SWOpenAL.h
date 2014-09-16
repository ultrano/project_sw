#ifndef SWOpenAL_h__
#define SWOpenAL_h__

#include "SWPlatform.h"
#include "SWPrimaryType.h"

#ifdef PLATFORM_WIN32
# include "al.h"
# include "alc.h"
#elif PLATFORM_IOS
# include <OpenAl/al.h>
# include <OpenAl/alc.h>
#elif PLATFORM_OSX

#elif PLATFORM_ANDROID

#endif

tuint alGenWaveBuffer( tbyte* buf, tuint bufSize );
bool alErrorLog();
#endif //! SWOpenAL_h__