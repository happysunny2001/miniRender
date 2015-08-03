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

#ifndef __CW_RENDER_NODE_H__
#define __CW_RENDER_NODE_H__

#include "Base/cwMacros.h"
#include "Ref/cwRef.h"
#include "Math/cwMath.h"
#include "Base/cwVector.h"
#include "Event/Interface/cwTouchEventInterface.h"
#include "Scheduler/cwScheduleInterface.h"

NS_MINIR_BEGIN

class cwEventListener;
class cwEffect;

class cwRenderNode : public cwRef, public cwTouchEventInterface, public cwScheduleInterface
{
public:
	static cwRenderNode* create();

	cwRenderNode();
	virtual ~cwRenderNode();

	virtual CWBOOL init();

	virtual CWVOID setParent(cwRenderNode* pNode);

	virtual CWVOID addChild(cwRenderNode* pNode);
	virtual CWVOID removeChild(cwRenderNode* pNode);
	virtual CWVOID removeChildren();

	const cwVector3D& getPosition() const { return m_nPos; }
	virtual CWVOID setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID setPosition(const cwVector3D& v);
	virtual CWVOID move(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID move(const cwVector3D& v);

	const cwVector3D& getRotation() const { return m_nRot; }
	virtual CWVOID setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID setRotation(const cwVector3D& v);
	virtual CWVOID rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID rotate(const cwVector3D& v);

	const cwVector3D& getScale() const { return m_nScale; }
	virtual CWVOID setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID setScale(const cwVector3D& v);
	virtual CWVOID scale(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual CWVOID scale(const cwVector3D& v);

	virtual CWVOID setVisible(CWBOOL b);
	CWBOOL getVisible() const { return m_bVisible; }

	virtual CWVOID setEffect(cwEffect* pEffect);
	inline cwEffect* getEffect() const { return m_pEffect; }

	virtual CWVOID transform();
	const cwMatrix4X4& getTransformMatrix() const { return m_nTrans; }

	virtual CWVOID addEventListener(cwEventListener* pListener);
	virtual CWVOID addEventListener(cwEventListener* pListener, CWINT iPriority, CWBOOL swallow);
	virtual CWVOID removeEventListerner(cwEventListener* pListener, CWBOOL bClean = CWTRUE);

	virtual CWVOID update(CWFLOAT dt);
	
	virtual CWVOID renderChildren();
	virtual CWVOID render();
	virtual CWVOID renderSelf();

protected:
	CWVOID clearChildren();
	CWVOID clearEventListener();
	
protected:
	CWBOOL m_bVisible;

	cwVector3D m_nPos;
	//angle of rotation around the x, y, z axis, in radians
	cwVector3D m_nRot;
	cwVector3D m_nScale;
	cwMatrix4X4 m_nTrans; 
	CWBOOL m_bTransDirty;

	cwEffect* m_pEffect;

	cwVector<cwRenderNode*> m_nVecChildren;
	cwRenderNode* m_pParent;

	cwVector<cwEventListener*> m_nVecEventListener;

};

NS_MINIR_END

#endif
