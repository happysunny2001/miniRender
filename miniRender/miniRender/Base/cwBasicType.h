#ifndef _CW_BASIC_TYPE_H__
#define _CW_BASIC_TYPE_H__

#include "cwMacros.h"
#include "Platform/cwPlatform.h"
#include <string>
using namespace std;

NS_MINI_BEGIN

typedef unsigned int     CWUINT;
typedef __int64          CWINT64;
typedef unsigned __int64 CWUINT64;
typedef double           CWDOUBLE;
typedef float            CWFLOAT;
typedef int              CWINT;
typedef bool             CWBOOL;
typedef char             CWCHAR;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
typedef wstring          CWWSTRING;
#endif

typedef std::string      CWSTRING;
typedef void             CWVOID;
typedef unsigned char    CWBYTE;
typedef CWVOID*          CWHANDLE;

NS_MINI_END

#endif