/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_RENDER_TARGET_PARSER_H__
#define __CW_RENDER_TARGET_PARSER_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Ref/cwRef.h"
#include "tinyxml2.h"

NS_MINIR_BEGIN

class cwRenderTexture;

class cwRenderTargetParser : public cwRef
{
public:
	static cwRenderTargetParser* create();

	cwRenderTargetParser();

	cwRenderTexture* parse(tinyxml2::XMLElement* pDomRenderTarget);

protected:
	cwRenderTexture* parseBackbuffer(tinyxml2::XMLElement* pDomRenderTarget);
	cwRenderTexture* parseTexture(tinyxml2::XMLElement* pDomRenderTarget);
	cwRenderTexture* parseShadowMap(tinyxml2::XMLElement* pDomRenderTarget);
	cwRenderTexture* parseMultiRenderTarget(tinyxml2::XMLElement* pDomRenderTarget);

};

NS_MINIR_END

#endif
