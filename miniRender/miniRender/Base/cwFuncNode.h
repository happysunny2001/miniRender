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

#ifndef __CW_FUNC_NODE_H__
#define __CW_FUNC_NODE_H__

#include "cwMacros.h"
#include <functional>

NS_MINIR_BEGIN

template<typename M, typename R, typename... Args>
class cwFuncNode
{
public:
	cwFuncNode(std::function<R(Args...)> func)
	{
		_func = func;
		//_calller = caller;
		//_funcPtr = funcPtr;
	}

	R operator()(Args... args)
	{
		return _func(args...);
	}

	//inline void* caller() { return _calller; }
	//inline F& funcPtr() { return _funcPtr; }
	inline M& data() { return _data; }

	inline void setData(const M& d) { _data = d; }

private:
	std::function<R(Args...)> _func;
	//void* _calller;
	//F _funcPtr;
	M _data;

};

NS_MINIR_END

#endif
