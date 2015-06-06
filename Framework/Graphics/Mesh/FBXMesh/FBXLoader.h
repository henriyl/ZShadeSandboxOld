//===============================================================================================================================
// FBXLoader
//
// http://www.walkerb.net/blog/dx-4/
// http://gamedev.stackexchange.com/questions/59419/c-fbx-animation-importer-using-the-fbx-sdk
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/how-to-work-with-fbx-sdk-r3582
// http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/skinned-mesh-animation-using-matrices-r3577
// https://github.com/lang1991/FBXExporter
//===============================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __FBXLOADER_H
#define __FBXLOADER_H
//==============================================================================================================================
//==============================================================================================================================
#include <fbxsdk.h>
#include <vector>
#include "CGlobal.h"
#include "Vertex.h"
using namespace std;
//==============================================================================================================================
//==============================================================================================================================
class FBXLoader
{
	FbxManager* m_pFBXManager;
	FbxScene* m_pFbxScene;
	string mAnimationName;
	uint32 mAnimationLength;
	
	vector* m_pControlPoints;
	vector* m_pOutVertices;
	vector* m_pOutIndices;
	
	// FBX Mesh Data
	
	void LoadVertexPosition(FbxMesh* mesh, int i);
	void LoadVertexNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& normal);
	
	// FBX Animation
	
	void ProcessSkeletonHierarchy(FbxNode* inRootNode);
	void ProcessSkeletonHierarchy(FbxNode* inNode, int myIndex, int parentIndex);
	void ProcessJointsAndAnimation(FbxNode* inNode);
	
	FbxAMatrix GetGeometryTransformation(FbxNode* inNode);
	
public:
	
	FBXLoader();
	
	bool Load(char* model_path, VertexType vt);
};
//==============================================================================================================================
//==============================================================================================================================
#endif//__FBXLOADER_H