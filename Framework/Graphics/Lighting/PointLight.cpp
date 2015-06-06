#include "PointLight.h"
#include "SphereMesh.h"
using ZShadeSandboxLighting::PointLight;
//==============================================================================================================================
//==============================================================================================================================
PointLight::PointLight()
{
}
//==============================================================================================================================
ZShadeSandboxLighting::Light* PointLight::Clone()
{
	PointLight* light = (PointLight*)new ZShadeSandboxLighting::Light();

	light->LightType() = mLightType;
	light->DiffuseColor() = mDiffuseColor;
	light->AmbientColor() = mAmbientColor;
	light->SpecularColor() = mSpecularColor;
	light->Position() = mPosition;
	light->Direction() = mDirection;
	light->ToggleSphereMesh() = bToggleSphereMesh;
	light->ToggleSphereMeshWireframe() = bToggleSphereMeshWireframe;
	light->Range() = fRange;
	light->Attenuation() = fAttenuation;

	// Need to clone the mesh
	//light->SetMesh((ZShadeSandboxMesh::SphereMesh*)mMesh->Clone());

	return light;
}
//==============================================================================================================================