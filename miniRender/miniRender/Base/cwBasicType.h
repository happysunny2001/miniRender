﻿/*
Copyright © 2015-2016 Ziwei Wang (happy.sunny.2001@163.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _CW_BASIC_TYPE_H__
#define _CW_BASIC_TYPE_H__

#include "cwMacros.h"
#include "Platform/cwPlatform.h"
#include <string>
using namespace std;

NS_MINIR_BEGIN

typedef __int64          CWINT64;
typedef unsigned __int64 CWUINT64;
typedef double           CWDOUBLE;
typedef float            CWFLOAT;
typedef unsigned __int32 CWUINT;
typedef __int32          CWINT;
typedef __int16          CWSHORT;
typedef CWSHORT          CWHALF;
typedef unsigned __int16 CWUSHORT;
typedef bool             CWBOOL;
typedef char             CWCHAR;
typedef unsigned char    CWBYTE;
typedef std::string      CWSTRING;
typedef void             CWVOID;
typedef CWVOID*          CWHANDLE;

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_
typedef wstring          CWWSTRING;
#else
typedef std::string      CWWSTRING;
#endif

#define CWTRUE           true
#define CWFALSE          false

NS_MINIR_END

#endif