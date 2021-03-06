﻿/*
Copyright © 2015-2016 Ziwei Wang

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

#ifndef __CW_GEOMETRYGENERATOR_H__
#define __CW_GEOMETRYGENERATOR_H__

#include "Base/cwUtils.h"
#include "Base/cwBasicType.h"
#include "Math/cwMath.h"
#include "Repertory/cwRepertory.h"
#include "Ref/cwRef.h"
#include <vector>

NS_MINIR_BEGIN

class cwRenderObject;
class cwEntity;

class CW_DLL cwGeometryGenerator : public cwRef
{
public:
	struct cwVertex {
		cwVertex() {}
		cwVertex(const cwVector3D& p, const cwVector3D& n, const cwVector3D& t, const cwVector2D& uv) :
			pos(p), normal(n), tangentU(t), tex(uv) {}
		cwVertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v
			) :
			pos(px, py, pz), normal(nx, ny, nz), tangentU(tx, ty, tz), tex(u, v) {}
		cwVector3D pos;
		cwVector3D normal;
		cwVector3D tangentU;
		cwVector2D tex;
	};

	struct cwMeshData {
		vector<cwVertex> nVertex;
		vector<CWUINT> nIndex;

		CWVOID buildTangent();
	};

	CWVOID generateGrid(CWFLOAT width, CWFLOAT height, CWUINT m, CWUINT n, cwMeshData& mesh);
	CWVOID generateBox(cwMeshData& mesh);
	CWVOID generateCylinder(
		CWFLOAT bottomRadius, CWFLOAT topRadius, 
		CWFLOAT height, 
		CWUINT sliceCount, CWUINT stackCount, 
		cwMeshData& mesh);
	CWVOID generateSphere(CWFLOAT radius, CWUINT sliceCount, CWUINT stackCount, cwMeshData& mesh);
	CWVOID generateGeoSphere(CWFLOAT radius, CWUINT divideTimes, cwMeshData& mesh);
	CWVOID generateQuad(cwMeshData& mesh);
	cwEntity* generateEntityQuad();

	CWVOID generateCoordinateAxis(CWFLOAT scale, cwMeshData& mesh);
	cwRenderObject* generateCoordinateAxisRenderObject(CWFLOAT scale);
	cwEntity* generateCoordinateAxisEntity(CWFLOAT scale);

	CWVOID loadTxt(const CWSTRING& strFile, cwMeshData& mesh);

	CWFLOAT getTerrainHeight(CWFLOAT x, CWFLOAT z);
	cwVector3D getTerrainNormal(CWFLOAT x, CWFLOAT z);

	cwRenderObject* defaultSpriteRenderObject();

private:
	static cwGeometryGenerator* create();
	cwGeometryGenerator();
	cwGeometryGenerator(cwGeometryGenerator const&){}
	cwGeometryGenerator& operator=(cwGeometryGenerator const&){}
	~cwGeometryGenerator();

	void buildCylinderCap(
		CWFLOAT bottomRadius, CWFLOAT topRadius,
		CWFLOAT height,
		CWUINT sliceCount, CWUINT stackCount,
		cwMeshData& mesh);

	void subDivide(cwMeshData& mesh);

	friend class cwRepertory;

private:
	cwRenderObject* m_pDefaultSpriteRenderObject;

};

NS_MINIR_END

#endif