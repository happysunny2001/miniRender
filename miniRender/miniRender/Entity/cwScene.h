﻿/*
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

#ifndef __CW_SCENE_H__
#define __CW_SCENE_H__

#include "Base/cwMacros.h"
#include "cwRenderNode.h"
#include "Base/cwVector.h"
#include "Light/cwLight.h"

NS_MINIR_BEGIN

class cwEntity;
class cwCamera;

class CW_DLL cwScene : public cwRenderNode
{
public:
	static cwScene* create();

	cwScene();
	virtual ~cwScene();

	virtual CWBOOL init() override;

	virtual CWVOID addLight(cwLight* pLight);
	virtual CWVOID removeLight(cwLight* pLight);
	const cwVector<cwLight*>& getLights() const;

	std::vector<cwEntity*>& getVisibleEntities(cwCamera* pCamera);

protected:
	cwVector<cwLight*> m_nVecLights;
	std::vector<cwEntity*> m_nVecVisibleEntity;

};

NS_MINIR_END

#endif
