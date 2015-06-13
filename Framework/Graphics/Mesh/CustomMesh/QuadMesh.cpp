#include "QuadMesh.h"
using ZShadeSandboxMesh::QuadMesh;
//===============================================================================================================================
//===============================================================================================================================
QuadMesh::QuadMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, bool updatable, bool fullScreen)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   bUpdatable(updatable)
,   bFullScreen(fullScreen)
{
	//Store the screen size
	if (updatable)
	{
		m_screenWidth = mEngineOptions->m_screenWidth;
		m_screenHeight = mEngineOptions->m_screenHeight;
	}
	else
	{
		m_screenWidth = mp.textureWidth;
		m_screenHeight = mp.textureHeight;
	}
	
	//Store the size in pixels that this bitmap should be rendered at
	m_bitmapWidth = mp.textureWidth;
	m_bitmapHeight = mp.textureHeight;
	
	//Initialize the previous rendering position to negative one
	m_previousPosX = -1;
	m_previousPosY = -1;
	
	mMeshType = ZShadeSandboxMesh::EMeshType::QUAD;

	InitializeMesh();
	//LoadVertexAdjacency();
	CreateBuffers();
}
//===============================================================================================================================
QuadMesh::~QuadMesh()
{
}
//===============================================================================================================================
void QuadMesh::UpdateBuffers(float positionX, float positionY)
{
	if (bUpdatable)
	{
		mAttributes->mPosition.x = positionX;
		mAttributes->mPosition.y = positionY;

		float left, right, top, bottom;

		//If the position we are rendering this bitmap to has not changed then don't update the vertex buffer
		//since it currently has the correct parameters
		if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
		{
			return;
		}

		//If it has changed then update the position it is being rendered to
		m_previousPosX = positionX;
		m_previousPosY = positionY;

		//Calculate the screen coordinates of the left side of the bitmap
		left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

		//Calculate the screen coordinates of the right side of the bitmap
		right = left + (float)m_bitmapWidth;

		//Calculate the screen coordinates of the top of the bitmap
		top = (float)(m_screenHeight / 2) - (float)positionY;

		//Calculate the screen coordinates of the bottom of the bitmap
		bottom = top - (float)m_bitmapHeight;

		//Load the vertex array with data
		switch (mMeshParameters.vertexType)
		{
			case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
			{
#pragma region "NormalTex"
				//First Triangle
				ZShadeSandboxMesh::VertexNormalTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(left, top, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(right, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				ZShadeSandboxMesh::VertexNormalTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(left, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexNormalTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(left, top, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(right, top, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(right, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				
				ZShadeSandboxMesh::VertexNormalTex verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesNormalTex.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesNormalTex[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
#pragma endregion
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
			{
#pragma region "NormalTexTan"
				//First Triangle
				ZShadeSandboxMesh::VertexNormalTexTan c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(left, top, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(right, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(left, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexNormalTexTan c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(left, top, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(right, top, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(right, bottom, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				
				ZShadeSandboxMesh::VertexNormalTexTan verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesNormalTexTan.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesNormalTexTan[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
#pragma endregion
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Pos:
			{
#pragma region "Pos"
				//First Triangle
				ZShadeSandboxMesh::VertexPos c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(left, top, 0.0f);
				ZShadeSandboxMesh::VertexPos c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(right, bottom, 0.0f);
				ZShadeSandboxMesh::VertexPos c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(left, bottom, 0.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexPos c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(left, top, 0.0f);
				ZShadeSandboxMesh::VertexPos c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(right, top, 0.0f);
				ZShadeSandboxMesh::VertexPos c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(right, bottom, 0.0f);
				
				ZShadeSandboxMesh::VertexPos verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesPos.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesPos[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
#pragma endregion
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Tex:
			{
#pragma region "Tex"
				//First Triangle
				ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(left, top, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(right, bottom, 0.0f, 1.0f, 1.0f);
				ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(left, bottom, 0.0f, 0.0f, 1.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(left, top, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(right, top, 0.0f, 1.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(right, bottom, 0.0f, 1.0f, 1.0f);
				
				ZShadeSandboxMesh::VertexTex verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesTex.clear();
				mAttributes->mVerticesTex.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesTex[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
#pragma endregion
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Color:
			{
#pragma region "Color"
				XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

				//First Triangle
				ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(left, top, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(right, bottom, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(left, bottom, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(left, top, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(right, top, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(right, bottom, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				
				ZShadeSandboxMesh::VertexColor verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesColor.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesColor[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
#pragma endregion
			}
			break;
		}
		
		// Regenerate the vertex buffer
		RegenVertexBuffer();
	}
}
//===============================================================================================================================
void QuadMesh::Initialize()
{
	if (bFullScreen)
	{
		InitFullscreenQuad();
	}
	else
	{
		//
		// Load Vertices
		//
		
		switch (mMeshParameters.vertexType)
		{
			case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
			{
				//First Triangle
				ZShadeSandboxMesh::VertexNormalTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				ZShadeSandboxMesh::VertexNormalTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexNormalTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				
				ZShadeSandboxMesh::VertexNormalTex verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesNormalTex.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesNormalTex[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
			{
				//First Triangle
				ZShadeSandboxMesh::VertexNormalTexTan c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexNormalTexTan c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexNormalTexTan c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
				
				ZShadeSandboxMesh::VertexNormalTexTan verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesNormalTexTan.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesNormalTexTan[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Pos:
			{
				//First Triangle
				ZShadeSandboxMesh::VertexPos c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexPos c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, 1.0f, 0.0f);
				ZShadeSandboxMesh::VertexPos c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, 1.0f, 0.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexPos c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexPos c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexPos c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, 1.0f, 0.0f);
				
				ZShadeSandboxMesh::VertexPos verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesPos.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesPos[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Tex:
			{
				//First Triangle
				ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
				ZShadeSandboxMesh::VertexTex c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
				
				ZShadeSandboxMesh::VertexTex verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesTex.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesTex[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
			}
			break;
			case ZShadeSandboxMesh::EVertexType::VT_Color:
			{
				//ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				//ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				//ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				//ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
				
				XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

				//First Triangle
				ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, 0.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, 1.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				
				//Second Triangle
				ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, 0.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c5 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 0.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				ZShadeSandboxMesh::VertexColor c6 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
				
				ZShadeSandboxMesh::VertexColor verts[NUM_QUAD_VERTS] =
				{
					c1, c2, c3, c4, c5, c6
				};
				
				mAttributes->mVerticesColor.resize( NUM_QUAD_VERTS );

				for (int i = 0; i < NUM_QUAD_VERTS; i++)
				{
					mAttributes->mVerticesColor[i] = verts[i];
				}

				mAttributes->mVertexCount = NUM_QUAD_VERTS;
			}
			break;
		}
		
		mAttributes->mTriangleCount = 2;
		
		//
		// Load Indices
		//
		
		mAttributes->mIndexCount = NUM_QUAD_INDICES;
		
		UINT indices[NUM_QUAD_INDICES] =
		{
			0, 1, 2, 3, 4, 5
			//0, 1, 2, 0, 2, 3
		};

		mAttributes->mIndices.resize(NUM_QUAD_INDICES);

		for (int i = 0; i < NUM_QUAD_INDICES; i++)
		{
			mAttributes->mIndices[i] = indices[i];
		}
	}
}
//===============================================================================================================================
void QuadMesh::InitTessellation()
{
	// Just creates a fullscreen quad
	InitFullscreenQuad();
}
//===============================================================================================================================
void QuadMesh::InitFullscreenQuad()
{
	//
	// Load Vertices
	//
	
	XMFLOAT3 v0(-1.0f,  1.0f, 0.0f);
	XMFLOAT3 v1( 1.0f,  1.0f, 0.0f);
	XMFLOAT3 v2(-1.0f, -1.0f, 0.0f);
	XMFLOAT3 v3( 1.0f, -1.0f, 0.0f);

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			ZShadeSandboxMesh::VertexNormalTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v0.x, v0.y, v0.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v1.x, v1.y, v1.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v2.x, v2.y, v2.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			ZShadeSandboxMesh::VertexNormalTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(v3.x, v3.y, v3.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesNormalTex.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesNormalTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			ZShadeSandboxMesh::VertexNormalTexTan c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v0.x, v0.y, v0.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v1.x, v1.y, v1.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v2.x, v2.y, v2.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(v3.x, v3.y, v3.z, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesNormalTexTan.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesNormalTexTan[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			ZShadeSandboxMesh::VertexPos c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v0.x, v0.y, v0.z);
			ZShadeSandboxMesh::VertexPos c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v1.x, v1.y, v1.z);
			ZShadeSandboxMesh::VertexPos c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v2.x, v2.y, v2.z);
			ZShadeSandboxMesh::VertexPos c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(v3.x, v3.y, v3.z);
			
			ZShadeSandboxMesh::VertexPos verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesPos.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesPos[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			ZShadeSandboxMesh::VertexTex c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v0.x, v0.y, v0.z, 0, 0);
			ZShadeSandboxMesh::VertexTex c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v1.x, v1.y, v1.z, 1, 0);
			ZShadeSandboxMesh::VertexTex c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v2.x, v2.y, v2.z, 0, 1);
			ZShadeSandboxMesh::VertexTex c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(v3.x, v3.y, v3.z, 1, 1);

			ZShadeSandboxMesh::VertexTex verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesTex.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesTex[i] = verts[i];
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->vDiffuseColor;

			ZShadeSandboxMesh::VertexColor c1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v0.x, v0.y, v0.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v1.x, v1.y, v1.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v2.x, v2.y, v2.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor c4 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(v3.x, v3.y, v3.z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[4] =
			{
				c1, c2, c3, c4
			};
			
			mAttributes->mVerticesColor.resize( 4 );

			for (int i = 0; i < 4; i++)
			{
				mAttributes->mVerticesColor[i] = verts[i];
			}
		}
		break;
	}
	
	mAttributes->mVertexCount = 4;
	
	mAttributes->mTriangleCount = 2;
	
	//
	// Load Indices
	//
	
	mAttributes->mIndexCount = 6;
	
	UINT indices[6] =
	{
		0, 1, 2, 3, 0, 2
	};
	
	mAttributes->mIndices.resize(6);
	
	for (int i = 0; i < 6; i++)
	{
		mAttributes->mIndices[i] = indices[i];
	}
}
//===============================================================================================================================