#include "FBXLoader.h"
#include "ZShadeMessageCenter.h"
//===============================================================================================================================
//===============================================================================================================================
FBXLoader::FBXLoader()
:   m_pFBXManager(nullptr)
{
}
//===============================================================================================================================
bool FBXLoader::Load(char* model_path, VertexType vt)
{
	int fileMinor, fileRevision;
	int sdkMajor, sdkMinor, sdkRevision;
	int fileFormat;
	
	if (m_pFBXManager == nullptr)
	{
		m_pFBXManager = FbxManager::Create();
		
		FbxIOSettings* pIOSettings = FbxIOSettings::Create(m_pFBXManager, IOSROOT);
		
		m_pFBXManager->SetIOSettings(pIOSettings);
	}
	
	FbxString filePath = FbxGetApplicationDirectory();
	m_pFBXManager->LoadPluginsDirectory(filePath.Buffer());
	
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);
	FbxImporter* pImporter = FbxImporter::Create(m_pFBXManager, "");
	m_pFbxScene = FbxScene::Create(m_pFBXManager, "");
	
	if(!m_pFBXManager->GetIOPluginRegistry()->DetectReaderFileFormat(model_path, fileFormat))
	{
		//Unrecognizable file format. Try to fall back on FbxImorter::eFBX_BINARY
		fileFormat = m_pFBXManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}
	
	bool bSuccess = pImporter->Initialize(model_path, fileFormat, m_pFBXManager->GetIOSettings());
	
	pImporter->GetFileVersion(fileMinor, fileMinor, fileRevision);
	
	if(!bSuccess)
	{
		printf( "ERROR %s : %d FbxImporter Initialize failed!\n", __FILE__, __LINE__ );
		return false;
	}
	
	bSuccess = pImporter->Import(m_pFbxScene);
	
	if (!bSuccess)
	{
		return false;
	}
	
	pImporter->Destroy();
	
	FbxNode* pFbxRootNode = m_pFbxScene->GetRootNode();
	
	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
			
			if (pFbxChildNode->GetNodeAttribute() == NULL)
			{
				continue;
			}
			
			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
			
			if (AttributeType != FbxNodeAttribute::eMesh)
			{
				continue;
			}
			
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			
			unsigned int ctrlPointCount = pMesh->GetControlPointsCount();
			for(unsigned int i = 0; i < ctrlPointCount; ++i)
			{
				LoadVertexPosition(pMesh, i);
			}
			
			/*FbxVector4* pVertices = pMesh->GetControlPoints();
			
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);
				
				assert( iNumVertices == 3 );
				
				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);
					
					LoadVertexPosition(pVertices, iControlPointIndex);
				}
				
			}*/
		}
	}
	
	return true;
}
//===============================================================================================================================
void FBXLoader::LoadVertexPosition(FbxMesh* mesh, int i)
{
	//position.x = (float)pVertices[index].mData[0];
	//position.y = (float)pVertices[index].mData[1];
	//position.z = (float)pVertices[index].mData[2];
	
	CtrlPoint* currCtrlPoint = new CtrlPoint();
	XMFLOAT3 position;
	position.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
	position.y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
	position.z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
	currCtrlPoint->mPosition = position;
	m_pControlPoints->push_back(currCtrlPoint);
}
//===============================================================================================================================
void FBXLoader::ReadVertexNormal(FbxMesh* mesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& normal)
{
	if (mesh->GetElementNormalCount() < 1)
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "FBXLoader: Invalid Normal Number");
		return;
	}
	
	FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
	
	switch (vertexNormal)
	{
		case FbxGeometryElement::eByControlPoint:
		{
			switch (vertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
					normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
					normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
					normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
					normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
					normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default:
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "FBXLoader: Invalid Reference");
				}
				break;
			}// end inner switch
		}
		break;
		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (vertexNormal->GetReferenceMode())
			{
				case FbxGeometryElement::eDirect:
				{
					normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
					normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
					normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
				}
				break;
				case FbxGeometryElement::eIndexToDirect:
				{
					int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
					normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
					normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
					normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
				}
				break;
				default:
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "FBXLoader: Invalid Reference");
				}
				break;
			}// end inner switch
		}
		break;
	}
}
//===============================================================================================================================
FbxAMatrix FBXLoader::GetGeometryTransformation(FbxNode* inNode)
{
	if (!inNode)
	{
		throw std::exception("Null for mesh geometry");
	}

	const FbxVector4 lT = inNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = inNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = inNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}
//===============================================================================================================================
void FBXLoader::ProcessSkeletonHierarchy(FbxNode* inRootNode)
{
	for (int childIndex = 0; childIndex < inRootNode->GetChildCount(); ++childIndex)
	{
		FbxNode* node = inRootNode->GetChild(childIndex);
		
		ProcessSkeletonHierarchy(node, 0, 0, -1);
	}
}
//===============================================================================================================================
void FBXLoader::ProcessSkeletonHierarchy(FbxNode* inNode, int myIndex, int parentIndex)
{
	if (inNode->GetNodeAttribute() &&
	    inNode->GetNodeAttribute()->GetAttributeType() &&
		inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.mParentIndex = parentIndex;
		currJoint.mName = inNode->GetName();
		mSkeleton.mJoints.push_back(currJoint);
	}
	
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchy(inNode->GetChild(i), mSkeleton.mJoints.size(), myIndex);
	}
}
//===============================================================================================================================
void FBXLoader::ProcessJointsAndAnimation(FbxNode* inNode)
{
	FbxMesh* mesh = inNode->GetMesh();
	
	uint32 numOfDeformers = mesh->GetDeformerCount();
	
	FbxAMatrix geomTrans = GetGeometryTransformation(inNode);
	
	// A deformer contains clusters.
	// A cluster contains a link, which is a joint.
	// Normally, There is only one deformer in a mesh but Maya has many types.
	for (uint32 deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// Lets see if this deformer is a skin
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin);
		
		if (!skin) continue;
		
		uint32 numOfClusters = skin->GetClusterCount();
		
		for (uint32 clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			
			string jointName = cluster->GetLink()->GetName();
			uint32 jointIndex = FindJointIndexByName(jointName);
			
			FbxAMatrix transform;
			FbxAMatrix transformLink;
			FbxAMatrix globalBindposeInverse;
			
			// The transformation of the mesh at binding time
			cluster->GetTransformMatrix(transform);
			
			// The transformation of the cluster (joint) at binding time from joint space to world space
			cluster->GetTransformLinkMatrix(transformLink);
			
			globalBindposeInverse = transformLink.Inverse() * transform * geomTrans;
			
			
			// Update skeletal information
			mSkeleton.mJoints[jointIndex].mGlobalBindposeInverse = globalBindposeInverse;
			mSkeleton.mJoints[jointIndex].mNode = cluster->GetLink();
			
			// Associate each joint with the control points it affects
			uint32 numOfIndices = cluster->GetControlPointIndicesCount();
			
			for (uint32 i = 0; i < numOfIndices; ++i)
			{
				//BlendingIndexWeightPair currBlendingIndexWeightPair;
				//currBlendingIndexWeightPair.mBlendingIndex = jointIndex;
				//currBlendingIndexWeightPair.mBlendingWeight = cluster->GetControlPointWeights()[i];
				//mControlPoints[cluster->GetControlPointIndices()[i]]->mBlendingInfo.push_back(currBlendingIndexWeightPair);
			}
			
			// Animation information
			FbxAnimStack* animStack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = animStack->GetName();
			mAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = m_pFbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			Keyframe** anim = &mSkeleton.mJoints[jointIndex].mAnimation;
			
			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime time;
				time.SetFrame(i, FbxTime::eFrames24);
				*anim = new Keyframe();
				(*anim)->mFrameNum = i;
				FbxAMatrix transformOffset = inNode->EvaluateGlobalTransform(time) * geomTrans;
				(*anim)->mGlobalTransform = currentTransformOffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
				anim = &((*anim)->mNext);
			}
		}
	}
	
	// Some control points have less than 4 joints
	//BlendingIndexWeightPair currBlendingIndexWeightPair;
	//currBlendingIndexWeightPair.mBlendingIndex = 0;
	//currBlendingIndexWeightPair.mBlendingWeight = 0;
	//for(auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	//{
	//	for(unsigned int i = itr->second->mBlendingInfo.size(); i <= 4; ++i)
	//	{
	//		itr->second->mBlendingInfo.push_back(currBlendingIndexWeightPair);
	//	}
	//}
}
//===============================================================================================================================
