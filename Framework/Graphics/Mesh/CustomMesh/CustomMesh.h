//===============================================================================================================================
// CustomMesh
//
//===============================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __CUSTOMMESH_H
#define __CUSTOMMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Vertex.h"
#include "CGlobal.h"
#include "Camera.h"
#include "PhysicsBody.h"
#include "Texture.h"
#include "TextureManager.h"
#include "CustomMeshParameters.h"
#include "ShaderMaterial.h"

// Shader includes
#include "MaterialShader.h"
#include "LightShader.h"
#include "ShadowMapBuildShader.h"
#include "QuadMaterialTessellationShader.h"
#include "TriMaterialTessellationShader.h"
#include "DeferredShader.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
//
// Globals
//

//This would be good for collision detection
namespace EMeshType
{
	enum Type
	{
		CUSTOM, //This is a mesh that is more complex like a grass field
		CUBE,
		TRI,
		QUAD,
		SPHERE,
		CYLINDER,
		GRID,
		GEOSPHERE,
		CIRCLE,
		PLANE
	};
}

//===============================================================================================================================
//===============================================================================================================================
class CustomMesh
{
public:

	CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename);
	CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp);
	~CustomMesh();

	virtual void Render(ZShadeSandboxMesh::MeshRenderParameters rp);
	//virtual void RenderClip(Camera* pCamera, XMMATRIX view, XMFLOAT4 clipplane, RenderType rt = TriangleList) {}
	void Update(float dt);
	virtual void Initialize() {}
	virtual void InitTessellation() {}
	
	void LoadFromFile(char* filename);

	// This function must be called to use tessellation
	void InitializeMesh();
	
	// Convenient functions to subdivide a custom mesh into smaller triangles
	void Subdivide_VertexPos();
	void GrabSubdividedGeometry_VertexPos(vector<ZShadeSandboxMesh::VertexPos> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexPos>& OutVerts, vector<UINT>& OutInd);
	void Subdivide_VertexTex();
	void GrabSubdividedGeometry_VertexTex(vector<ZShadeSandboxMesh::VertexTex> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexTex>& OutVerts, vector<UINT>& OutInd);
	void Subdivide_VertexColor();
	void GrabSubdividedGeometry_VertexColor(vector<ZShadeSandboxMesh::VertexColor> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexColor>& OutVerts, vector<UINT>& OutInd);
	void Subdivide_VertexNormalTexTan();
	void GrabSubdividedGeometry_VertexNormalTexTan(vector<ZShadeSandboxMesh::VertexNormalTexTan> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexNormalTexTan>& OutVerts, vector<UINT>& OutInd);
	void Subdivide_VertexNormalTex();
	void GrabSubdividedGeometry_VertexNormalTex(vector<ZShadeSandboxMesh::VertexNormalTex> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexNormalTex>& OutVerts, vector<UINT>& OutInd);

	void ReplyMeshIndices(vector<UINT> ind);
	
	//For dynamic buffer to change values at runtime
	void RegenVertexBuffer();

	void SetBuffers(ERenderType::Type renderType);
	//void SetBuffersNoIndex(RenderType rt);

	void Shade(ZShadeSandboxMesh::MeshRenderParameters mrp);
	
	//void SetTexture(ID3D11ShaderResourceView* texSRV);// { pTexture = texSRV; }

	XMMATRIX WorldXM();
	
	XMFLOAT3& Position() { return mPosition; }
	XMFLOAT3& Rotation() { return mRotation; }
	XMFLOAT3& Scale() { return mScale; }
	
	XMFLOAT3 Position() const { return mPosition; }
	XMFLOAT3 Rotation() const { return mRotation; }
	XMFLOAT3 Scale() const { return mScale; }
	
	bool& RenderShader() { return m_RenderShader; }
	bool RenderShader() const { return m_RenderShader; }
	
	EMeshType::Type MeshType() { return mMeshType; }
	
	// Adds the forward light buffers to the material
	void SetLightBuffer(ZShadeSandboxLighting::LightBuffer* lb);
	void SetLightBuffer(ZShadeSandboxLighting::SunLightBuffer* slb);
	
	void SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV);
	string TextureName() const;

	void RemoveTexture(int i);

	//Texture*& MeshTexture() { return mTexture; }
	//Texture* MeshTexture() const { return mTexture; }
	
	ZShadeSandboxLighting::ShaderMaterial* GetMaterial() { return mMaterial; }

	ZShadeSandboxMesh::MeshParameters& MeshParams() { return mMeshParameters; }
	ZShadeSandboxMesh::MeshParameters MeshParams() const { return mMeshParameters; }

	//Returns a billboard rotation to the camera
	void BillboardWorldXM(Camera* camera);

	//void SetClipPlane(XMFLOAT4 clipplane) { mClipPlane = clipplane; }

	void SetWireframe(bool set) { m_Wireframe = set; }

	UINT GetIndexCount() { return mIndexCount; }
	int TriangleCount() const { return mTriangleCount; }
	
	void SetShadowMapSRV(ID3D11ShaderResourceView* srv) { mMaterial->SetShadowMapTexture(srv); }
	void SetSSAOMapSRV(ID3D11ShaderResourceView* srv) { mMaterial->SetSSAOTexture(srv); }
	void EnableShadowMap(bool enable) { mMaterial->EnableShadowMap() = enable; }
	void EnableSSAOMap(bool enable) { mMaterial->EnableSSAOMap() = enable; }

	// Physics
	
	// Does this mesh collide with another mesh
	bool Collides(CustomMesh* mesh);
	
	// Check whether or not a point will enter the AABB of this body
	bool IntersectsAABB(XMFLOAT3 point);
	// Gets the distance from the AABB of this body to a point
	float DistanceAABBToPoint(XMFLOAT3 point);
	
	ZShadeSandboxMath::AABB* GetAABB();
	void UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale);

	// Update the physics body if it is a dynamic body
	void UpdatePhysicsBody();
	
	void CreatePhysicsBody();
	
	PhysicsBody* GetPhysicsBody();
	

	virtual CustomMesh* Clone() { return NULL; }

protected:
	void CreateBuffers();
	
	bool m_RenderShader;
	
	ZShadeSandboxMesh::MeshParameters mMeshParameters;

	EMeshType::Type mMeshType;
	ERenderType::Type mRenderType;

	//string mTextureName;
	//Texture* mTexture;
	//Texture* mCloudTexture;
	//Texture* mCloudPerturbTexture;
	
	bool m_Wireframe;

	bool m_TranslateBillboard;

	// Just so we can load a mesh that does not require an index buffer
	bool m_DisableIndexBuffer;
	
	// Used to apply a color across all vertices
	//XMFLOAT4   mDefaultColor;
	
	XMFLOAT3   mPosition;
	XMFLOAT3   mRotation;
	XMFLOAT3   mScale;
	XMFLOAT4X4 mWorld;
	XMMATRIX   mBillboardWorld;
	
	ZShadeSandboxLighting::ShaderMaterial* mMaterial;

	// These shaders should only be created one time
	static MaterialShader*					pMaterialShader;
	static LightShader*						pLightShader;
	static QuadMaterialTessellationShader* 	pQuadMaterialTessellationShader;
	static TriMaterialTessellationShader* 	pTriMaterialTessellationShader;
	static ShadowMapBuildShader*	 		pShadowMapBuildShader;
	static DeferredShader*					pDeferredShader;
	
	//static TextureShader*            pTextureShader;
	//static ColorShader*              pColorShader;
	//static TransparentShader*        pTransparentShader;
	//static QuadColorTessellationShader* pQuadColorTessellationShader;
	//static QuadTextureTessellationShader* pQuadTextureTessellationShader;
	//static TriColorTessellationShader* pTriColorTessellationShader;
	//static TriTextureTessellationShader* pTriTextureTessellationShader;
	
	vector<ZShadeSandboxMesh::VertexNormalTex>		mVerticesVNT;
	vector<ZShadeSandboxMesh::VertexNormalTexTan>	mVerticesVNTT;
	vector<ZShadeSandboxMesh::VertexPos>			mVerticesPos;
	vector<ZShadeSandboxMesh::VertexTex>			mVerticesTex;
	vector<ZShadeSandboxMesh::VertexColor>			mVerticesColor;
	
	vector<UINT>              mIndices;
	
	//The physics body of the mesh
	PhysicsBody* mBody;
	
	D3D*                      pD3DSystem;
	EngineOptions*            pEngineOptions;
	ID3D11Buffer*             pVB;
	ID3D11Buffer*             pIB;
	UINT                      mVertexByteWidth;
	UINT                      mVertexCount;
	UINT                      mIndexCount;
	int                       mTriangleCount;
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__CUSTOMMESH_H