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

class cwRenderNode : public cwRef, public cwTouchEventInterface, public cwScheduleInterface
{
public:
	static cwRenderNode* create();

	cwRenderNode();
	virtual ~cwRenderNode();

	virtual bool init();

	virtual void setParent(cwRenderNode* pNode);

	virtual void addChild(cwRenderNode* pNode);
	virtual void removeChild(cwRenderNode* pNode);
	virtual void removeChildren();

	const cwVector3D& getPosition() const { return m_nPos; }
	virtual void setPosition(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void setPosition(const cwVector3D& v);
	virtual void move(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void move(const cwVector3D& v);

	const cwVector3D& getRotation() const { return m_nRot; }
	virtual void setRotation(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void setRotation(const cwVector3D& v);
	virtual void rotate(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void rotate(const cwVector3D& v);

	const cwVector3D& getScale() const { return m_nScale; }
	virtual void setScale(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void setScale(const cwVector3D& v);
	virtual void scale(CWFLOAT x, CWFLOAT y, CWFLOAT z);
	virtual void scale(const cwVector3D& v);

	virtual void setVisible(bool b);
	bool getVisible() const { return m_bVisible; }

	virtual void transform();
	const cwMatrix4X4& getTransformMatrix() const { return m_nTrans; }

	virtual void addEventListener(cwEventListener* pListener);
	virtual void addEventListener(cwEventListener* pListener, CWINT iPriority, bool swallow);
	virtual void removeEventListerner(cwEventListener* pListener, bool bClean=true);

	virtual void update(CWFLOAT dt);
	
	virtual void renderChildren();
	virtual void render();
	virtual void renderSelf();

protected:
	void clearChildren();
	void clearEventListener();
	
protected:
	bool m_bVisible;

	cwVector3D m_nPos;
	//angle of rotation around the x, y, z axis, in radians
	cwVector3D m_nRot;
	cwVector3D m_nScale;
	cwMatrix4X4 m_nTrans; 

	cwVector<cwRenderNode*> m_nVecChildren;
	cwRenderNode* m_pParent;

	cwVector<cwEventListener*> m_nVecEventListener;

};

NS_MINIR_END

#endif
