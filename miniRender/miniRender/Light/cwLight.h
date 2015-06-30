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

#ifndef __CW_LIGHT_H__
#define __CW_LIGHT_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Ref/cwRef.h"

NS_MINIR_BEGIN

class CW_DLL cwLight : public cwRef
{
public:
	static cwLight* createDirectionalLight(
		const cwVector3D& direction,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular);

	static cwLight* createPointLight(
		const cwVector3D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector3D& attenuation,
		CWFLOAT range
		);

	static cwLight* createSpotLight(
		const cwVector3D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector3D& attenuation,
		CWFLOAT range,
		const cwVector3D& direction,
		CWFLOAT spot
		);

	cwLight();
	~cwLight();

	virtual bool initDirectionalLight(
		const cwVector3D& direction,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular);

	virtual bool initPointLight(
		const cwVector3D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector3D& attenuation,
		CWFLOAT range);

	virtual bool initSpotLight(
		const cwVector3D& position,
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector3D& attenuation,
		CWFLOAT range,
		const cwVector3D& direction,
		CWFLOAT spot);

	void setPosition(const cwVector3D& pos);
	const cwVector3D& getPosition();

	void setDirection(const cwVector3D& dir);
	const cwVector3D& getDirection();

	void setAttenuation(const cwVector3D& att);
	const cwVector3D getAttenuation();

	void setRange(CWFLOAT f);
	CWFLOAT getRange();

	void setSpotLightExponent(CWFLOAT f);
	CWFLOAT getSpotLightExponent();

	void setSpotDirection(const cwVector3D& dir);
	const cwVector3D getSpotDirection();

	void setAmbient(const cwVector4D& color);
	const cwVector4D& getAmbient();

	void setDiffuse(const cwVector4D& color);
	const cwVector4D& getDiffuse();

	void setSpecular(const cwVector4D& color);
	const cwVector4D& getSpecular();

	const CWUINT size() const;
	CWVOID* data() const;

	void setIndex(CWUINT index) { m_nIndex = index; }
	const CWUINT getIndex() const { return m_nIndex; }

	void setType(eLightType type);
	const eLightType getType() const;

	void setTag(CWUINT iTag) { m_iTag = iTag; }
	const CWUINT getTag() const { return m_iTag; }

	void setName(const CWSTRING& strName) { m_strName = strName; }
	const CWSTRING& getName() const { return m_strName; }

public:
	union {
		//directional light
		struct {
			cwVector3D direction;
			CWFLOAT type;
			cwVector4D ambient;
			cwVector4D diffuse;
			cwVector4D specular;
		};
		//point light
		struct {
			cwVector3D position;
			CWFLOAT type;
			cwVector4D ambient;
			cwVector4D diffuse;
			cwVector4D specular;
			cwVector4D att; //x,y,z:attenuation A0, A1, A2; w:range
		};
		//spot light
		struct {
			cwVector3D position;
			CWFLOAT type;
			cwVector4D ambient;
			cwVector4D diffuse;
			cwVector4D specular;
			cwVector4D att; //x,y,z:attenuation A0, A1, A2; w:range
			cwVector4D spotDirection; //x, y, z:spot light position; w:spot
		};
	};

protected:
	CWUINT m_nIndex; //light index in shader's light array
	CWUINT m_iTag;
	CWSTRING m_strName;

};

NS_MINIR_END

#endif