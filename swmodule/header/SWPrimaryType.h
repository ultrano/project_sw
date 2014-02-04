
#ifndef sw_SWPrimaryType_h
#define sw_SWPrimaryType_h
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "SWMemory.h"

typedef std::basic_string<char, std::char_traits<char>, SWAllocator<char> > tstring2;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, SWAllocator<wchar_t> > twstring2;
typedef std::string    tstring;
typedef std::wstring   twstring;
typedef double         tnumber;
typedef bool           tboolean;
typedef int            tint;
typedef short          tshort;
typedef char           tchar;
typedef float          tfloat;
typedef unsigned int   tuint;
typedef unsigned short tushort;
typedef unsigned char  tbyte;
typedef tint           tint32;
typedef tuint          tuint32;
typedef tshort         tint16;
typedef tushort        tuint16;
typedef tchar          tint8;
typedef tbyte          tuint8;
typedef tuint32        thash32;

#define tlist  std::list
#define ttable std::map
#define tset   std::set
#define tarray std::vector
#define tpair  std::pair

#endif //! sw_SWPrimaryType_h