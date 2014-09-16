#ifndef SWOpanAL_h__
#define SWOpanAL_h__

#include "SWPlatform.h"
#include "SWPrimaryType.h"

#ifdef PLATFORM_WIN32
# include "al.h"
# include "alc.h"
# include "alext.h"
#elif PLATFORM_IOS

#elif PLATFORM_OSX

#elif PLATFORM_ANDROID

#endif

tuint alGenWaveBuffer( tbyte* buf, tuint bufSize );

#endif //! SWOpanAL_h__