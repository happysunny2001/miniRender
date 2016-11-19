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

#ifndef __CW_EFFECT_MANAGER_H__
#define __CW_EFFECT_MANAGER_H__

#include "Base/cwMacros.h"
#include "Base/cwBasicType.h"
#include "Base/cwMap.h"
#include "Ref/cwRef.h"
#include "Repertory/cwRepertory.h"
#include "cwEffect.h"
#include "cwEffectConstant.h"

NS_MINIR_BEGIN

class cwEffectManager : public cwRef
{
public:
	cwEffect* defaultSpriteEffect();
	cwEffect* getDefEffect(eDefEffectID effectID);

private:
	static cwEffectManager* create();
	cwEffectManager();
	cwEffectManager(cwEffectManager const&){}
	cwEffectManager& operator=(cwEffectManager const&){}
	~cwEffectManager();

	CWVOID loadDefEffect();

	friend class cwRepertory;

private:
	cwEffect* m_pDefaultSpriteEffect;
	cwMap<eDefEffectID, cwEffect*> m_nMapDefEffect;

};

NS_MINIR_END

#endif
