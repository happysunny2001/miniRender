/*
Copyright © 2015 Ziwei Wang

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

#ifndef __CW_EFFECT_H__
#define __CW_EFFECT_H__

#include "Base/cwMacros.h"
#include "Base/cwVector.h"
#include "Ref/cwRef.h"
#include "Shader/cwShader.h"
#include "cwEffectParameter.h"

NS_MINIR_BEGIN

class cwEffect : public cwRef
{
public:
	static cwEffect* create();

	cwEffect();
	virtual ~cwEffect();

	virtual CWVOID setShader(cwShader* pShader);
	inline cwShader* getShader() const { return m_pShader; }

	virtual CWVOID setTech(const CWSTRING& strTech);
	inline const CWSTRING& getTech() const { return m_strTech; }

	CWVOID addParameter(cwEffectParameter* pEffectParameter);
	virtual CWVOID config();

protected:
	cwShader* m_pShader;
	CWSTRING m_strTech;

	cwVector<cwEffectParameter*> m_nVecParameter;

};

NS_MINIR_END

#endif
