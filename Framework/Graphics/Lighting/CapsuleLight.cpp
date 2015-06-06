#include "CapsuleLight.h"
#include "SphereMesh.h"
using ZShadeSandboxLighting::CapsuleLight;
//==============================================================================================================================
//==============================================================================================================================
CapsuleLight::CapsuleLight()
{
}
//==============================================================================================================================
ZShadeSandboxLighting::Light* CapsuleLight::Clone()
{
	CapsuleLight* light = (CapsuleLight*)new ZShadeSandboxLighting::Light();

	light->LightType() = mLightType;
	light->DiffuseColor() = mDiffuseColor;
	light->AmbientColor() = mAmbientColor;
	light->SpecularColor() = mSpecularColor;
	light->Position() = mPosition;
	light->Direction() = mDirection;
	light->ToggleSphereMesh() = bToggleSphereMesh;
	light->ToggleSphereMeshWireframe() = bToggleSphereMeshWireframe;
	light->Range() = fRange;
	light->LightLength() = fLightLength;
	light->CapsuleIntensity() = fCapsuleIntensity;
	light->CapsuleDirectionLength() = fCapsuleDirectionLength;
	
	// Need to clone the mesh
	//light->SetMesh((ZShadeSandboxMesh::SphereMesh*)mMesh->Clone());

	return light;
}
//==============================================================================================================================