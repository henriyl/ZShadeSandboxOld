#include "DirectionalLight.h"
#include "SphereMesh.h"
using ZShadeSandboxLighting::DirectionalLight;
//==============================================================================================================================
//==============================================================================================================================
DirectionalLight::DirectionalLight()
{
}
//==============================================================================================================================
ZShadeSandboxLighting::Light* DirectionalLight::Clone()
{
	DirectionalLight* light = (DirectionalLight*)new ZShadeSandboxLighting::Light();

	light->LightType() = mLightType;
	light->DiffuseColor() = mDiffuseColor;
	light->AmbientColor() = mAmbientColor;
	light->SpecularColor() = mSpecularColor;
	light->Position() = mPosition;
	light->Direction() = mDirection;
	light->ToggleSphereMesh() = bToggleSphereMesh;
	light->ToggleSphereMeshWireframe() = bToggleSphereMeshWireframe;

	// Need to clone the mesh
	//light->SetMesh((ZShadeSandboxMesh::SphereMesh*)mMesh->Clone());

	return light;
}
//==============================================================================================================================