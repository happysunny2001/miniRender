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

#include "cwGeometryGenerator.h"
#include "Math/cwMath.h"

#include <fstream>
#include <iostream>
using namespace std;

NS_MINI_BEGIN

cwGeometryGenerator& cwGeometryGenerator::getInstance()
{
	static cwGeometryGenerator gInstance;
	return gInstance;
}

void cwGeometryGenerator::generateGrid(CWFLOAT width, CWFLOAT height, CWUINT m, CWUINT n, cwMeshData& mesh)
{
	CWUINT vertexCnt = m*n;
	CWFLOAT halfWidth  = width*0.5f;
	CWFLOAT halfHeight = height*0.5f;
	CWFLOAT widthStep  = width / (CWFLOAT)(n-1);
	CWFLOAT heightStep = height / (CWFLOAT)(m-1);
	CWFLOAT uStep = 1.0f / (CWFLOAT)(n-1);
	CWFLOAT vStep = 1.0f / (CWFLOAT)(m-1);

	//build vertex
	mesh.nVertex.resize(vertexCnt);
	for (CWUINT i = 0; i < m; ++i) {
		float z = halfHeight - i*heightStep;
		for (CWUINT j = 0; j < n; ++j) {
			cwVertex& v = mesh.nVertex[i*n+j];
			v.pos      = cwVector3D(-halfWidth + j*widthStep, 0, z);
			v.normal   = cwVector3D(0, 1.0f, 0);
			v.tangentU = cwVector3D(1.0, 0, 0);
			v.tex      = cwVector2D(j*uStep, i*vStep);
		}
	}

	//build index
	//face count = square count * 2
	CWUINT faceCnt = (m - 1)*(n - 1) * 2;
	CWUINT k = 0;
	mesh.nIndex.resize(faceCnt*3);
	for (CWUINT i = 0; i < m - 1; ++i) {
		for (CWUINT j = 0; j < n - 1; ++j) {
			mesh.nIndex[k] = i*n + j;
			mesh.nIndex[k + 1] = i*n + j+1;
			mesh.nIndex[k + 2] = (i + 1)*n + j;
			mesh.nIndex[k + 3] = (i + 1)*n + j;
			mesh.nIndex[k + 4] = i*n + j + 1;
			mesh.nIndex[k + 5] = (i + 1)*n + j+1;
			k += 6;
		}
	}
}

void cwGeometryGenerator::generateBox(cwMeshData& mesh)
{
	mesh.nVertex.resize(24);

	mesh.nVertex[0] = cwVertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	mesh.nVertex[1] = cwVertex(-0.5f, +0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	mesh.nVertex[2] = cwVertex(+0.5f, +0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	mesh.nVertex[3] = cwVertex(+0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	mesh.nVertex[4] = cwVertex(-0.5f, -0.5f, +0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	mesh.nVertex[5] = cwVertex(+0.5f, -0.5f, +0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	mesh.nVertex[6] = cwVertex(+0.5f, +0.5f, +0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	mesh.nVertex[7] = cwVertex(-0.5f, +0.5f, +0.5f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	mesh.nVertex[8]  = cwVertex(-0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	mesh.nVertex[9]  = cwVertex(-0.5f, +0.5f, +0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	mesh.nVertex[10] = cwVertex(+0.5f, +0.5f, +0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	mesh.nVertex[11] = cwVertex(+0.5f, +0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	mesh.nVertex[12] = cwVertex(-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	mesh.nVertex[13] = cwVertex(+0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	mesh.nVertex[14] = cwVertex(+0.5f, -0.5f, +0.5f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	mesh.nVertex[15] = cwVertex(-0.5f, -0.5f, +0.5f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	mesh.nVertex[16] = cwVertex(-0.5f, -0.5f, +0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	mesh.nVertex[17] = cwVertex(-0.5f, +0.5f, +0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	mesh.nVertex[18] = cwVertex(-0.5f, +0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	mesh.nVertex[19] = cwVertex(-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	mesh.nVertex[20] = cwVertex(+0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	mesh.nVertex[21] = cwVertex(+0.5f, +0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	mesh.nVertex[22] = cwVertex(+0.5f, +0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	mesh.nVertex[23] = cwVertex(+0.5f, -0.5f, +0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	mesh.nIndex.resize(36);
	mesh.nIndex[0]  = 0; mesh.nIndex[1]  = 1; mesh.nIndex[2]  = 2;
	mesh.nIndex[3]  = 0; mesh.nIndex[4]  = 2; mesh.nIndex[5]  = 3;

	mesh.nIndex[6]  = 4; mesh.nIndex[7]  = 5; mesh.nIndex[8]  = 6;
	mesh.nIndex[9]  = 4; mesh.nIndex[10] = 6; mesh.nIndex[11] = 7;

	mesh.nIndex[12] = 8; mesh.nIndex[13] = 9;  mesh.nIndex[14] = 10;
	mesh.nIndex[15] = 8; mesh.nIndex[16] = 10; mesh.nIndex[17] = 11;

	mesh.nIndex[18] = 12; mesh.nIndex[19] = 13; mesh.nIndex[20] = 14;
	mesh.nIndex[21] = 12; mesh.nIndex[22] = 14; mesh.nIndex[23] = 15;

	mesh.nIndex[24] = 16; mesh.nIndex[25] = 17; mesh.nIndex[26] = 18;
	mesh.nIndex[27] = 16; mesh.nIndex[28] = 18; mesh.nIndex[29] = 19;

	mesh.nIndex[30] = 20; mesh.nIndex[31] = 21; mesh.nIndex[32] = 22;
	mesh.nIndex[33] = 20; mesh.nIndex[34] = 22; mesh.nIndex[35] = 23;
}

void cwGeometryGenerator::generateCylinder(
	CWFLOAT bottomRadius, CWFLOAT topRadius,
	CWFLOAT height,
	CWUINT sliceCount, CWUINT stackCount,
	cwMeshData& mesh)
{
	mesh.nVertex.clear();
	mesh.nIndex.clear();

	float fStackHeight = height / stackCount;
	float fRadiusStep = (topRadius - bottomRadius) / stackCount;
	CWUINT ringCnt = stackCount + 1;

	for (CWUINT j = 0; j < ringCnt; ++j) {
		float fY = -0.5f*height + j*fStackHeight;
		float fR = bottomRadius + j*fRadiusStep;
		float fTheta = 2.0f*cwMathUtil::cwPI / sliceCount;

		for (CWUINT i = 0; i < sliceCount + 1; ++i) {
			cwVertex vertex;
			float c = cosf(i*fTheta);
			float s = sinf(i*fTheta);
			vertex.pos = cwVector3D(c*fR, fY, s*fR);
			vertex.tex.x = (CWFLOAT)i / sliceCount;
			vertex.tex.y = 1.0f - (CWFLOAT)j / stackCount;
			vertex.tangentU = cwVector3D(-s, 0, c);
			float dr = bottomRadius - topRadius;
			cwVector3D bitangent(dr*c, -height, dr*s);
			vertex.normal = vertex.tangentU.cross(bitangent);
			mesh.nVertex.push_back(vertex);
		}
	}

	CWUINT ringVertexCnt = sliceCount + 1;
	for (CWUINT j = 0; j < stackCount; ++j) {
		for (CWUINT i = 0; i < sliceCount; ++i) {
			mesh.nIndex.push_back(j*ringVertexCnt + i);
			mesh.nIndex.push_back((j+1)*ringVertexCnt + i);
			mesh.nIndex.push_back((j+1)*ringVertexCnt + i+1);

			mesh.nIndex.push_back(j*ringVertexCnt + i);
			mesh.nIndex.push_back((j + 1)*ringVertexCnt + i + 1);
			mesh.nIndex.push_back(j*ringVertexCnt + i+1);
		}
	}

	buildCylinderCap(bottomRadius, topRadius, height, sliceCount, stackCount, mesh);
}

void cwGeometryGenerator::buildCylinderCap(
	CWFLOAT bottomRadius, CWFLOAT topRadius,
	CWFLOAT height,
	CWUINT sliceCount, CWUINT stackCount,
	cwMeshData& mesh)
{
	//top
	CWUINT baseIndex = (CWUINT)mesh.nVertex.size();
	float fY = 0.5f*height;
	float fTheta = 2.0f*cwMathUtil::cwPI / sliceCount;

	for (CWUINT i = 0; i < sliceCount + 1; ++i) {
		float fX = topRadius * cosf(i*fTheta);
		float fZ = topRadius * sinf(i*fTheta);

		float u = fX / height + 0.5f;
		float v = fZ / height + 0.5f;
		mesh.nVertex.push_back(cwVertex(fX, fY, fZ, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	//top center vertex
	mesh.nVertex.push_back(cwVertex(0, fY, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));
	CWUINT centerIndex = (CWUINT)(mesh.nVertex.size() - 1);
	for (CWUINT i = 0; i < sliceCount; ++i) {
		mesh.nIndex.push_back(centerIndex);
		mesh.nIndex.push_back(baseIndex + i + 1);
		mesh.nIndex.push_back(baseIndex + i);
	}

	//bottom
	baseIndex = (CWUINT)mesh.nVertex.size();
	fY = -0.5f*height;

	for (CWUINT i = 0; i < sliceCount + 1; ++i) {
		float fX = bottomRadius * cosf(i*fTheta);
		float fZ = bottomRadius * sinf(i*fTheta);

		float u = fX / height + 0.5f;
		float v = fZ / height + 0.5f;
		mesh.nVertex.push_back(cwVertex(fX, fY, fZ, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
	}

	//bottom center vertex
	mesh.nVertex.push_back(cwVertex(0, fY, 0, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));
	centerIndex = (CWUINT)(mesh.nVertex.size() - 1);
	for (CWUINT i = 0; i < sliceCount; ++i) {
		mesh.nIndex.push_back(centerIndex);
		mesh.nIndex.push_back(baseIndex + i);
		mesh.nIndex.push_back(baseIndex + i+1);
	}
}

void cwGeometryGenerator::generateSphere(CWFLOAT radius, CWUINT sliceCount, CWUINT stackCount, cwMeshData& mesh)
{
	mesh.nVertex.clear();
	mesh.nIndex.clear();

	cwVertex topVertex(0, radius, 0, 0, 1.0f, 0, 1.0f, 0, 0, 0, 0);
	cwVertex bottomVertex(0, -radius, 0, 0, -1.0f, 0, 1.0f, 0, 0, 0, 1.0f);

	mesh.nVertex.push_back(topVertex);
	float fTheta = 2.0f*cwMathUtil::cwPI / sliceCount;
	float fPhi = cwMathUtil::cwPI / stackCount;

	for (CWUINT j = 1; j < sliceCount; ++j) {
		float phi = j*fPhi;
		for (CWUINT i = 0; i <= stackCount; ++i) {
			float theta = i*fTheta;

			cwVertex v;
			// spherical to cartesian
			v.pos.x = radius*sinf(phi)*cosf(theta);
			v.pos.z = radius*sinf(phi)*sinf(theta);
			v.pos.y = radius*cosf(phi);

			v.tangentU.x = -radius*sinf(phi)*sinf(theta);
			v.tangentU.y = 0;
			v.tangentU.z = radius*sinf(phi)*cosf(theta);
			v.tangentU.normalize();

			v.normal = v.pos;
			v.normal.normalize();

			v.tex.x = theta / (2.0f*cwMathUtil::cwPI);
			v.tex.y = phi / cwMathUtil::cwPI;

			mesh.nVertex.push_back(v);
		}
	}

	mesh.nVertex.push_back(bottomVertex);

	for (CWUINT i = 1; i <= sliceCount; ++i) {
		mesh.nIndex.push_back(0);
		mesh.nIndex.push_back(i+1);
		mesh.nIndex.push_back(i);
	}

	CWUINT baseIndex = 1;
	CWUINT vertexPerRing = sliceCount + 1;
	for (CWUINT j = 0; j < stackCount - 2; ++j) {
		for (CWUINT i = 0; i < sliceCount; ++i) {
			mesh.nIndex.push_back(baseIndex + j*vertexPerRing + i);
			mesh.nIndex.push_back(baseIndex + j*vertexPerRing + i + 1);
			mesh.nIndex.push_back(baseIndex + (j + 1)*vertexPerRing + i);

			mesh.nIndex.push_back(baseIndex + (j + 1)*vertexPerRing + i);
			mesh.nIndex.push_back(baseIndex + j*vertexPerRing + i + 1);
			mesh.nIndex.push_back(baseIndex + (j+1)*vertexPerRing + i+1);
		}
	}

	CWUINT bottomVertexIndex = (CWUINT)(mesh.nVertex.size() - 1);
	baseIndex = bottomVertexIndex - vertexPerRing;
	for (CWUINT i = 0; i < sliceCount; ++i) {
		mesh.nIndex.push_back(bottomVertexIndex);
		mesh.nIndex.push_back(baseIndex + i);
		mesh.nIndex.push_back(baseIndex + i+1);
	}
}

void cwGeometryGenerator::generateGeoSphere(CWFLOAT radius, CWUINT divideTimes, cwMeshData& mesh)
{
	CWFLOAT fX = 0.525731f;
	CWFLOAT fZ = 0.850651f;

	cwVector3D pos[12] = {
		cwVector3D(-fX, 0.0f, fZ),  cwVector3D(fX, 0.0f, fZ),
		cwVector3D(-fX, 0.0f, -fZ), cwVector3D(fX, 0.0f, -fZ),
		cwVector3D(0.0f, fZ, fX),   cwVector3D(0.0f, fZ, -fX),
		cwVector3D(0.0f, -fZ, fX),  cwVector3D(0.0f, -fZ, -fX),
		cwVector3D(fZ, fX, 0.0f),   cwVector3D(-fZ, fX, 0.0f),
		cwVector3D(fZ, -fX, 0.0f),  cwVector3D(-fZ, -fX, 0.0f)
	};

	CWUINT k[60] = {
		1, 4,  0,     4, 9, 0,     4, 5, 9,      8, 5, 4,      1, 8, 4,
		1, 10, 8,    10, 3, 8,     8, 3, 5,      3, 2, 5,      3, 7, 2,
		3, 10, 7,    10, 6, 7,     6, 11, 7,     6, 0, 11,     6, 1, 0,
		10, 1, 6,    11, 0, 9,     2, 11, 9,     5, 2, 9,     11, 2, 7
	};

	mesh.nVertex.resize(12);
	mesh.nIndex.resize(60);

	for (CWUINT i = 0; i < 12; ++i) {
		mesh.nVertex[i].pos = pos[i];
	}

	for (CWUINT i = 0; i < 60; ++i) {
		mesh.nIndex[i] = k[i];
	}
	
	for (CWUINT i = 0; i < divideTimes; ++i)
		subDivide(mesh);

	float fDivRadius = 1.0f / radius;
	for (CWUINT i = 0; i < mesh.nVertex.size(); ++i) {
		mesh.nVertex[i].normal = mesh.nVertex[i].pos;
		mesh.nVertex[i].normal.normalize();
		mesh.nVertex[i].pos = mesh.nVertex[i].normal*radius;

		float theta = cwMathUtil::angleFrom(mesh.nVertex[i].pos.x, mesh.nVertex[i].pos.z);
		float phi = acosf(mesh.nVertex[i].pos.y * fDivRadius);

		mesh.nVertex[i].tex.x = theta / cwMathUtil::cwPIx2;
		mesh.nVertex[i].tex.y = phi / cwMathUtil::cwPI;

		cwVector3D tangent(-radius*sinf(phi)*sinf(theta), 0, radius*sinf(phi)*cosf(theta));
		tangent.normalize();
		mesh.nVertex[i].tangentU = tangent;

	}
}

void cwGeometryGenerator::subDivide(cwMeshData& mesh)
{
	cwMeshData oldMesh = mesh;

	//
	//
	//        v1
	//      /   \
	//     /     \
	//    m0------m1
	//   / \     /  \
	//  /   \   /    \
	// v0-----m2-----v2

	mesh.nVertex.resize(0);
	mesh.nIndex.resize(0);

	CWUINT numTri = (CWUINT)oldMesh.nIndex.size() / 3;
	for (CWUINT i = 0; i < numTri; ++i) {
		cwVertex v0 = oldMesh.nVertex[oldMesh.nIndex[i * 3]];
		cwVertex v1 = oldMesh.nVertex[oldMesh.nIndex[i * 3 + 1]];
		cwVertex v2 = oldMesh.nVertex[oldMesh.nIndex[i * 3 + 2]];

		cwVertex m0, m1, m2;
		m0.pos = cwVector3D(0.5f*(v0.pos.x + v1.pos.x), 0.5f*(v0.pos.y + v1.pos.y), 0.5f*(v0.pos.z + v1.pos.z));
		m1.pos = cwVector3D(0.5f*(v1.pos.x + v2.pos.x), 0.5f*(v1.pos.y + v2.pos.y), 0.5f*(v1.pos.z + v2.pos.z));
		m2.pos = cwVector3D(0.5f*(v0.pos.x + v2.pos.x), 0.5f*(v0.pos.y + v2.pos.y), 0.5f*(v0.pos.z + v2.pos.z));

		mesh.nVertex.push_back(v0); //0
		mesh.nVertex.push_back(v1); //1
		mesh.nVertex.push_back(v2); //2
		mesh.nVertex.push_back(m0); //3
		mesh.nVertex.push_back(m1); //4
		mesh.nVertex.push_back(m2); //5

		mesh.nIndex.push_back(i * 6 + 0);
		mesh.nIndex.push_back(i * 6 + 3);
		mesh.nIndex.push_back(i * 6 + 5);

		mesh.nIndex.push_back(i * 6 + 3);
		mesh.nIndex.push_back(i * 6 + 1);
		mesh.nIndex.push_back(i * 6 + 4);

		mesh.nIndex.push_back(i * 6 + 3);
		mesh.nIndex.push_back(i * 6 + 4);
		mesh.nIndex.push_back(i * 6 + 5);

		mesh.nIndex.push_back(i * 6 + 5);
		mesh.nIndex.push_back(i * 6 + 4);
		mesh.nIndex.push_back(i * 6 + 2);
	}
}

void cwGeometryGenerator::loadTxt(const std::string& strFile, cwMeshData& mesh)
{
	std::ifstream fin(strFile);

	if (!fin) {
		cout << "open " << strFile << " error!" << endl;
		return;
	}

	CWUINT vertexCnt, triangleCnt;
	string nouse;

	fin >> nouse >> vertexCnt; //VertexCount: N
	fin >> nouse >> triangleCnt; //TriangleCount: N
	fin >> nouse >> nouse >> nouse; //VertexList (pos, normal)
	fin >> nouse; //{

	for (CWUINT i = 0; i < vertexCnt; ++i) {
		cwVertex vertex;
		fin >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z;
		fin >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;

		mesh.nVertex.push_back(vertex);
	}

	fin >> nouse; //}
	fin >> nouse; //TriangleList
	fin >> nouse; //{

	for (CWUINT i = 0; i < triangleCnt; ++i) {
		CWUINT i0, i1, i2;

		fin >> i0 >> i1 >> i2;
		mesh.nIndex.push_back(i0);
		mesh.nIndex.push_back(i1);
		mesh.nIndex.push_back(i2);
	}

	fin.close();
}

CWFLOAT cwGeometryGenerator::getTerrainHeight(CWFLOAT x, CWFLOAT z)
{
	return 0.3f*(z*sinf(0.1f*x) + x*cosf(0.1f*z));
}

cwVector3D cwGeometryGenerator::getTerrainNormal(CWFLOAT x, CWFLOAT z)
{
	// n = (-df/dx, 1, -df/dz)
	cwVector3D n(
		-0.03f*z*cosf(0.1f*x) - 0.3f*cosf(0.1f*z),
		1.0f,
		-0.3f*sinf(0.1f*x) + 0.03f*x*sinf(0.1f*z));
	n.normalize();

	return n;
}

NS_MINI_END