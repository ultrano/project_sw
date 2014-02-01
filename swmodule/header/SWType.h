//
//  SWType.h
//  prototype
//
//  Created by 한상운 on 13. 5. 25..
//  Copyright (c) 2013년 한상운. All rights reserved.
//

#ifndef sw_SWType_h
#define sw_SWType_h
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
typedef unsigned int   tuint;
typedef unsigned short tushort;
typedef unsigned char  tbyte;

#define tlist  std::list
#define ttable std::map
#define tset   std::set
#define tarray std::vector
#define tpair  std::pair

#endif //! sw_SWType_h
