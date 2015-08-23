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

#ifndef __CW_SPOT_LIGHT_H__
#define __CW_SPOT_LIGHT_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Ref/cwRef.h"
#include "cwLight.h"

NS_MINIR_BEGIN

class cwSpotLight : public cwLight
{
public:
	static cwSpotLight* create(
		const cwVector4D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& attenuation,
		const cwVector4D& direction);

	cwSpotLight();
	virtual ~cwSpotLight();

	virtual CWBOOL init(
		const cwVector4D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& attenuation,
		const cwVector4D& direction);

	inline CWVOID setPosition(const cwVector4D& pos) { m_nPosition = pos; }
	inline const cwVector4D& getPosition() const { return m_nPosition; }

	inline CWVOID setAmbient(const cwVector4D& color) { m_nAmbient = color; }
	inline const cwVector4D& getAmbient() const{ return m_nAmbient; }

	inline CWVOID setDiffuse(const cwVector4D& color) { m_nDiffuse = color; }
	inline const cwVector4D& getDiffuse() const { return m_nDiffuse; }

	inline CWVOID setSpecular(const cwVector4D& color) { m_nSpecular = color; }
	inline const cwVector4D& getSpecular() const { return m_nSpecular; }

	inline CWVOID setAttenuation(const cwVector4D& att) {
		m_nAtt.x = att.x;
		m_nAtt.y = att.y;
		m_nAtt.z = att.z;
	}
	const cwVector4D& getAttenuation() const { return m_nAtt; }

	inline CWVOID setRange(CWFLOAT f) { m_nAtt.w = f; }
	CWFLOAT getRange() const { return m_nAtt.w; }

	inline CWVOID setSpotDirection(const cwVector4D& dir) {
		m_nSpotDirection.x = dir.x;
		m_nSpotDirection.y = dir.y;
		m_nSpotDirection.z = dir.z;
	}
	inline const cwVector4D& getSpotDirection() const { return m_nSpotDirection; }

	inline CWVOID setSpotLightExponent(CWFLOAT f) { m_nSpotDirection.w = f; }
	CWFLOAT getSpotLightExponent() const { return m_nSpotDirection.w; }

	virtual CWUINT size() const override;
	virtual CWVOID* data() const override;

protected:
	cwVector4D m_nPosition;
	cwVector4D m_nAmbient;
	cwVector4D m_nDiffuse;
	cwVector4D m_nSpecular;
	cwVector4D m_nAtt; //x,y,z:attenuation A0, A1, A2; w:range
	cwVector4D m_nSpotDirection; //x, y, z:spot light direction; w:spot

};

NS_MINIR_END

#endif
