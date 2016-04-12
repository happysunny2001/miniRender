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

#include "cwColor.h"

NS_MINIR_BEGIN

cwVector4D cwColor::black( 0.0f, 0.0f, 0.0f, 1.0f );
cwVector4D cwColor::white( 1.0f, 1.0f, 1.0f, 1.0f );
cwVector4D cwColor::blue( 0.0f, 0.0f, 1.0f, 1.0f );
cwVector4D cwColor::red( 1.0f, 0.0f, 0.0f, 1.0f );
cwVector4D cwColor::green( 0.0f, 1.0f, 0.0f, 1.0f );
cwVector4D cwColor::yellow( 1.0f, 1.0f, 0.0f, 1.0f );
cwVector4D cwColor::sliver( 0.75f, 0.75f, 0.75f, 1.0f );

cwColor::cwColor()
{
}

cwColor::~cwColor()
{
}

NS_MINIR_END
