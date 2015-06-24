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

#ifndef __CW_CWMATERIAL_H__
#define __CW_CWMATERIAL_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Ref/cwRef.h"

NS_MINI_BEGIN

class cwShader;
class cwTexture;
class cwBlend;

class CW_DLL cwMaterial : public cwRef
{
public:
	struct matColor {
		cwVector4D m_nAmbient;  //ambient color
		cwVector4D m_nDiffuse;
		cwVector4D m_nSpecular; //w: specular power
		cwVector4D m_nReflect;
	};

public:
	static cwMaterial* create(
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& reflect,
		const string& strShader);

	static cwMaterial* create(
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& reflect);


	cwMaterial();
	virtual ~cwMaterial();

	virtual bool init(
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& reflect,
		const string& strShader);

	virtual bool init(
		const cwVector4D& ambient,
		const cwVector4D& diffuse,
		const cwVector4D& specular,
		const cwVector4D& reflect);

	virtual void setAmbient(const cwVector4D& color);
	inline const cwVector4D& getAmbient() { return m_nMatData.m_nAmbient; }

	virtual void setDiffuse(const cwVector4D& color);
	inline const cwVector4D& getDiffuse() { return m_nMatData.m_nDiffuse; }

	virtual void setSpecular(const cwVector4D& color);
	inline const cwVector4D& getSpecular() { return m_nMatData.m_nSpecular; }

	virtual void setReflect(const cwVector4D& color);
	inline const cwVector4D& getReflect() { return m_nMatData.m_nReflect; }

	virtual void setShader(const string& strShader);
	inline cwShader* getShader() { return m_pShader; }

	virtual void setDiffuseTexture(cwTexture* pTexture);
	virtual void setDiffuseTexture(const string& strTexName);
	inline cwTexture* getDiffuseTexture() { return m_pDiffuseTexture; }

	virtual void setBlend(cwBlend* pBlendOp);
	inline const cwBlend* getBlend() const { return m_pBlendOp; }

	inline CWVOID* getColorData() { return &m_nMatData;  }
	inline CWUINT getColorDataSize() { return sizeof(matColor); }

	virtual void setTechName(const string& str) { m_strTechName = str; }
	inline const string& getTechName() const { return m_strTechName; }

	virtual void configEffect();

public:
	matColor m_nMatData;
	cwShader* m_pShader;
	string m_strTechName;         //the name of technique for render 
	cwTexture* m_pDiffuseTexture;
	cwBlend* m_pBlendOp;          //blend object
	
};

NS_MINI_END

#endif