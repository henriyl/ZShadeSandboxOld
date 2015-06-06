#include "Light.h"
#include "SphereMesh.h"
#include "MaterialManager.h"
using ZShadeSandboxLighting::Light;
//==============================================================================================================================
//==============================================================================================================================
Light::Light()
{
	mMesh = NULL;
	mLightCamera = new LightCamera();
}
//==============================================================================================================================
void Light::BuildSphereMesh(D3D* d3d)
{
	float size = 10;
	float r = size / 100;
	
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = mPosition;
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(size, size, size);
	mp.material = MaterialManager::Instance()->GetMaterial("Stone");
	mp.in2D = false;
	mp.textureWidth = -1;
	mp.textureHeight = -1;
	mp.shader = 0;
	
	mMesh = new ZShadeSandboxMesh::SphereMesh(r, size, size, d3d, mp);
}
//==============================================================================================================================
void Light::AddMeshLightBuffer(ZShadeSandboxLighting::LightBuffer* lb)
{
	if (mMesh != NULL)
		mMesh->SetLightBuffer(lb);
}
//==============================================================================================================================
void Light::AddMeshLightBuffer(ZShadeSandboxLighting::SunLightBuffer* sb)
{
	if (mMesh != NULL)
		mMesh->SetLightBuffer(sb);
}
//==============================================================================================================================
void Light::UpdateLVP()
{
	mLightCamera->Update(mDirection);
}
//==============================================================================================================================
void Light::Update()
{
	mLightCamera->Position() = mDirection;
	mLightCamera->Update();
}
//==============================================================================================================================
void Light::SetLens(float fnear, float ffar)
{
	mLightCamera->Near() = fnear;
	mLightCamera->Far() = ffar;
	mLightCamera->LookAt() = XMFLOAT3(0, 0, 0);
	mLightCamera->SetLens();
}
//==============================================================================================================================
void Light::UpdateMeshPosition(XMFLOAT3 pos)
{
	mPosition = pos;
	mMesh->Position() = pos;
}
//==============================================================================================================================
void Light::ScaleMesh(XMFLOAT3 scale)
{
	mMesh->Scale() = scale;
}
//==============================================================================================================================
int Light::TriangleCount() const { return mMesh->TriangleCount(); }
//==============================================================================================================================
void Light::RenderSphereMesh(Camera* camera, LightCamera* lightcamera, bool reflect, XMFLOAT4 clipplane)
{
	if (mMesh == NULL) return;

	if (bToggleSphereMesh)
	{
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		mrp.pLightCamera = lightcamera;
		mrp.bReflection = reflect;
		mrp.clipplane = clipplane;
		mrp.bRenderDeferred = bToggleRenderDeferredMesh;
		mMesh->SetWireframe(bToggleSphereMeshWireframe);
		mMesh->Render(mrp);
	}
}
//==============================================================================================================================
void Light::SetMesh(ZShadeSandboxMesh::CustomMesh* mesh)
{
	mMesh = mesh;
}
//==============================================================================================================================
int Light::FetchLightTypeIndex()
{
	switch (mLightType)
	{
		case ZShadeSandboxLighting::ELightType::eAmbient: return 0;
		case ZShadeSandboxLighting::ELightType::eDirectional: return 1;
		case ZShadeSandboxLighting::ELightType::eSpot: return 2;
		case ZShadeSandboxLighting::ELightType::ePoint: return 3;
		case ZShadeSandboxLighting::ELightType::eCapsule: return 4;
		default: break;
	}

	return -1;
}
//==============================================================================================================================