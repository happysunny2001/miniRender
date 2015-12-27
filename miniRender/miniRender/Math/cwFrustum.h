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

#ifndef __CW_FRUSTUM_H_
#define __CW_FRUSTUM_H_

#include "Base/cwBasicType.h"
#include "Base/cwMacros.h"
#include "cwPlane.h"
#include "cwAABB.h"

NS_MINIR_BEGIN

class cwCamera;

class cwFrustum
{
public:
	enum FrustumPlane{
		LeftPlane = 0,
		RightPlane,
		TopPlane,
		BottomPlane,
		NearPlane,
		FarPlane,
	};

public:
	cwFrustum();
	~cwFrustum();

	void refresh(cwCamera* pCamera);
	inline const cwPlane& getPlane(FrustumPlane plane) const { return m_nPlanes[plane]; }
	inline const cwPlane* getPlane() const { return m_nPlanes; }

	//intersection with aabb
	//return value: lower 8 bit means whether outside the plane, 0 is outside the plane
	//			    9-16 bit means inside or intersect the plane, 0 is intersect the plane, 1 is inside the plane
	int intersection(const cwAABB& aabb) const;
	int intersection(const cwCircle& circle) const;

	//whether collide, inside or intersect the frustum 
	inline bool isCollide(int iRet) const { return (iRet & 0x00FF) == 0x003F; }
	//whether inside the frustum
	inline bool isInside(int iRet) const { return (iRet & 0xFFFF) == 0x3F3F; }

protected:
	cwPlane m_nPlanes[6];

};

NS_MINIR_END

#endif
