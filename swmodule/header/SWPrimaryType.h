
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

typedef bool           tboolean;

typedef float          tfloat;
typedef double         tdouble;

typedef tfloat         tfloat32;
typedef double         tdouble64;

typedef tfloat32       treal32;
typedef tdouble64      treal64;

typedef short          tshort;
typedef unsigned short tushort;

typedef char           tchar;
typedef unsigned char  tbyte;

typedef int            tint;
typedef tchar          tint8;
typedef tshort         tint16;
typedef tint           tint32;
typedef long long int  tint64;

typedef unsigned int   tuint;
typedef tbyte          tuint8;
typedef tushort        tuint16;
typedef tuint          tuint32;
typedef unsigned long long int tuint64;

typedef tuint32        thash32;
typedef tuint64        thash64;


#define tlist  std::list
#define ttable std::map
#define tset   std::set
#define tarray std::vector
#define tpair  std::pair


#define abstract virtual
#define override virtual
#define interface class

//! argument usage definitions, using with function argument.
//! such as pointer or reference value (ex. int* a; int& b )
#define _in_    //! function don't set a value to argument, only use.
#define _out_   //! function don't use argument, only for output.
#define _inout_ //! function do both using and setting.
//! [example]
//! void inputFunc( _in_ int& value );
//! void outputFunc( _out_ int& value );
//! void bothFunc( _inout_ int& value );

#endif //! sw_SWPrimaryType_h
