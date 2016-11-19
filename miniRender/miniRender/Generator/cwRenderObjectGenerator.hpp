/*
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

#ifndef __CW_RENDER_OBJECT_GENERATOR_H__
#define __CW_RENDER_OBJECT_GENERATOR_H__

#include "Base/cwMacros.h"
#include "cwGeometryGenerator.h"
#include "Repertory/cwRepertory.h"
#include "RenderObject/cwStaticRenderObject.h"

NS_MINIR_BEGIN

class cwRenderObject;

template<typename T>
class CW_DLL cwRenderObjectGenerator
{
public:
	cwRenderObjectGenerator() {}
	~cwRenderObjectGenerator() {}

	cwRenderObject* generateQuad(const string& strLayout);
	cwRenderObject* generateGrid(CWFLOAT fWidth, CWFLOAT fHeight, CWUINT m, CWUINT n, const string& strLayout);
	cwRenderObject* generateBox(const string& strLayout);
	cwRenderObject* generateSphere(CWFLOAT radius, CWUINT sliceCount, CWUINT stackCount, const string& strLayout);

protected:

};

template<typename T>
cwRenderObject* cwRenderObjectGenerator<T>::generateQuad(const string& strLayout)
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateQuad(mesh);

	vector<T> vecVertex(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecVertex[i] = mesh.nVertex[i];
	}

	cwRenderObject* pRenderObj = nullptr;
	pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecVertex[0], sizeof(T), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strLayout);

	return pRenderObj;
}

template<typename T>
cwRenderObject* cwRenderObjectGenerator<T>::generateGrid(CWFLOAT fWidth, CWFLOAT fHeight, CWUINT m, CWUINT n, const string& strLayout)
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateGrid(fWidth, fHeight, m, n, mesh);

	vector<T> vecPlane(mesh.nVertex.size());
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecPlane[i] = mesh.nVertex[i];
	}

	cwRenderObject* pRenderObj = nullptr;
	pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecPlane[0], sizeof(T), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strLayout);

	return pRenderObj;
}

template<typename T>
cwRenderObject* cwRenderObjectGenerator<T>::generateBox(const string& strLayout)
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateBox(mesh);

	vector<T> vecBox(mesh.nVertex.size());
	for (int i = 0; i < mesh.nVertex.size(); ++i) {
		vecBox[i] = mesh.nVertex[i];
	}

	cwRenderObject* pRenderObj = nullptr;
	pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecBox[0], sizeof(T), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strLayout);

	return pRenderObj;
}

template<typename T>
cwRenderObject* cwRenderObjectGenerator<T>::generateSphere(CWFLOAT radius, CWUINT sliceCount, CWUINT stackCount, const string& strLayout)
{
	cwGeometryGenerator::cwMeshData mesh;
	cwRepertory::getInstance().getGeoGenerator()->generateSphere(radius, sliceCount, stackCount, mesh);

	vector<T> vecSphere(mesh.nVertex.size());
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		vecSphere[i] = mesh.nVertex[i];
	}

	cwRenderObject* pRenderObj = nullptr;
	pRenderObj = cwStaticRenderObject::create(
		ePrimitiveTypeTriangleList,
		(CWVOID*)&vecSphere[0], sizeof(T), static_cast<CWUINT>(mesh.nVertex.size()),
		(CWVOID*)&(mesh.nIndex[0]), static_cast<CWUINT>(mesh.nIndex.size()), strLayout);

	return pRenderObj;
}

NS_MINIR_END

#endif
