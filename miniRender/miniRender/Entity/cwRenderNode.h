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
#include "Base/cwUtils.h"
#include "Ref/cwRef.h"
#include "Math/cwMath.h"
#include "Base/cwVector.h"
#include "Event/Interface/cwTouchEventInterface.h"
#include "Event/Interface/cwKeyboardEventInterface.h"
#include "Scheduler/cwScheduleInterface.h"

NS_MINIR_BEGIN

class cwEventListener;
class cwEffect;

class CW_DLL cwRenderNode : public cwRef, public cwTouchEventInterface, public cwKeyboardEventInterface, public cwScheduleInterface
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

	inline eSceneObjectType getType() const { return m_eType; }
	inline CWVOID setType(eSceneObjectType eType) { m_eType = eType; }

	virtual CWVOID setEffect(cwEffect* pEffect);
	inline cwEffect* getEffect() const { return m_pEffect; }

	inline CWBOOL getTransDirty() const { return m_bTransDirty; }
	inline cwRenderNode* getParent() const { return m_pParent; }

	cwVector<cwRenderNode*>& getChildren() { return m_nVecChildren; }

	virtual CWVOID refreshBoundingBox();
	inline const cwAABB& getBoundingBox() const { return m_nBoundingBox; }

	virtual CWVOID refreshGroupBoundingBox();
	inline const cwAABB& getGroupBoundingBox() const { return m_nGroupBoundingBox; }

	virtual CWVOID transform();
	virtual CWVOID refreshTransform();
	const cwMatrix4X4& getTransformMatrix() const { return m_nTrans; }
	CWVOID setTransformMatrix(const cwMatrix4X4& mat);
	const cwMatrix4X4& getLocalTransMatrix() const { return m_nLocalTrans; }

	virtual CWVOID addEventListener(cwEventListener* pListener);
	virtual CWVOID addEventListener(cwEventListener* pListener, CWINT iPriority, CWBOOL swallow);
	virtual CWVOID removeEventListerner(cwEventListener* pListener, CWBOOL bClean = CWTRUE);

	virtual CWVOID update(CWFLOAT dt);
	
	virtual CWVOID render();

protected:
	CWVOID clearChildren();
	CWVOID clearEventListener();
	CWVOID updateChildrenTransform();
	
protected:
	eSceneObjectType m_eType;
	CWBOOL m_bVisible;

	cwVector3D m_nPos;
	//angle of rotation around the x, y, z axis, in radians
	cwVector3D m_nRot;
	cwVector3D m_nScale;
	cwMatrix4X4 m_nLocalTrans;
	cwMatrix4X4 m_nTrans; 
	CWBOOL m_bTransDirty;

	cwEffect* m_pEffect; //effect for render, contains shader and tech

	cwVector<cwRenderNode*> m_nVecChildren;
	cwRenderNode* m_pParent;

	cwVector<cwEventListener*> m_nVecEventListener;

	cwAABB m_nBoundingBox;
	cwAABB m_nGroupBoundingBox;

};

NS_MINIR_END

#endif
