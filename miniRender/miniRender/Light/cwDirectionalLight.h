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

#ifndef __CW_DIRECTIONAL_LIGHT_H__
#define __CW_DIRECTIONAL_LIGHT_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Ref/cwRef.h"
#include "cwLight.h"

NS_MINIR_BEGIN

class CW_DLL cwDirectionalLight : public cwLight
{
public:
	struct DirectionalLight_Struct
	{
		cwVector4D m_nDirection;
		cwVector3D m_nColor;
		CWFLOAT m_fAttr; //shadow flag, 0:not cast shadow, 1:cast shadow, default is 0
	};
public:
	//static cwDirectionalLight* create(
	//	const cwVector4D& direction,
	//	const cwVector4D& ambient,
	//	const cwVector4D& diffuse,
	//	const cwVector4D& specular);

	static cwDirectionalLight* create(
		const cwVector4D& direction, 
		const cwVector3D& color);

	cwDirectionalLight();
	cwDirectionalLight(cwDirectionalLight& light);
	virtual ~cwDirectionalLight();

	//virtual CWBOOL init(
	//	const cwVector4D& direction,
	//	const cwVector4D& ambient,
	//	const cwVector4D& diffuse,
	//	const cwVector4D& specular);

	virtual CWBOOL init(
		const cwVector4D& direction,
		const cwVector3D& color);

	inline CWVOID setDirection(const cwVector4D& dir) { 
		m_nDirectionalLight.m_nDirection = dir; 
		m_nDirectionalLight.m_nDirection.w = 0;
	}
	inline const cwVector4D& getDirection() const { return m_nDirectionalLight.m_nDirection; }

	//inline CWVOID setAmbient(const cwVector4D& color) { m_nAmbient = color; }
	//inline const cwVector4D& getAmbient() const{ return m_nAmbient; }

	//inline CWVOID setDiffuse(const cwVector4D& color) { m_nDiffuse = color; }
	//inline const cwVector4D& getDiffuse() const { return m_nDiffuse; }

	//inline CWVOID setSpecular(const cwVector4D& color) { m_nSpecular = color; }
	//inline const cwVector4D& getSpecular() const { return m_nSpecular; }

	virtual CWUINT size() const override;
	virtual CWVOID* data() const override;
	cwDirectionalLight::DirectionalLight_Struct& getData() { return m_nDirectionalLight; }

	virtual CWVOID setCastShadow(CWBOOL b) override;
	virtual CWBOOL getCastShadow() const override;

protected:
	//cwVector4D m_nDirection;
	//cwVector4D m_nAmbient;
	//cwVector4D m_nDiffuse;
	//cwVector4D m_nSpecular;
	//cwVector4D m_nLightAttr; //x:shadow flag, 0:not cast shadow, 1:cast shadow, default is 0

	DirectionalLight_Struct m_nDirectionalLight;

};

NS_MINIR_END

#endif
