/*
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

#ifndef __CW_MACROS_H__
#define __CW_MACROS_H__

#include "Platform/cwPlatform.h"

#if _CW_PLATFORM_ == _CW_PLATFORM_WINDOWS_

#define CW_DLL //__declspec(dllexport)

#endif

#define NS_MINIR_BEGIN namespace minir {
#define NS_MINIR_END };
#define NS_USING_MINIR using namespace minir

#define CW_DEBUG 1

#define CW_SAFE_RETAIN(o)\
do{\
	if ((o))\
		(o)->retain();\
} while (0)

#define CW_SAFE_RELEASE(o) \
do{\
if ((o))\
	(o)->release(); \
} while (0)

#define CW_SAFE_RELEASE_NULL(o) \
do{\
	if ((o)) {\
		(o)->release(); \
		(o) = nullptr; \
	}\
} while (0)

#define CW_SAFE_DELETE_ARRAY(o) \
do{ \
if ((o)) { \
		delete[](o); \
		(o) = NULL; \
}\
} while (0)

#define CW_SAFE_DELETE(o) \
do {\
	if ((o)) { \
		delete (o); \
		(o) = NULL; \
	}\
} while (0);

#define CW_SAFE_FREE(o) \
do {\
	if ((o)) {\
		free((o)); \
		(o) = NULL; \
	}\
} while (0);

#define CW_CALLBACK_0(__selector__, __target__, ...) std::bind(&__selector__, __target__, ##__VA_ARGS__)
#define CW_CALLBACK_1(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, ##__VA_ARGS__)
#define CW_CALLBACK_2(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)

#define CW_STRING_MATCH(SRC, DEST) (strlen((SRC))==strlen((DEST)) && strncmp((SRC), (DEST), strlen((DEST)))==0 )

#endif
