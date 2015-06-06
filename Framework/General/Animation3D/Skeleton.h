//==================================================================================================================================
// Skeleton.h
//
//==================================================================================================================================
// History
//
// -Created on 4/2/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __SKELETON_H
#define __SKELETON_H
//==================================================================================================================================
//==================================================================================================================================
#include <fbxsdk.h>
#include <iostream>
#include <string>
#include <vector>
#include "Keyframe.h"
using namespace std;
//==================================================================================================================================
struct Joint
{
	std::string mName;
	int mParentIndex;
	FbxAMatrix mGlobalBindposeInverse;
	Keyframe* mAnimation;
	FbxNode* mNode;

	Joint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		while(mAnimation)
		{
			Keyframe* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}
};
//==================================================================================================================================
struct Skeleton
{
	std::vector<Joint> mJoints;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__SKELETON_H