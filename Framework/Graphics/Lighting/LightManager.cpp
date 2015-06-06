#include "LightManager.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "CapsuleLight.h"
#include "LightShadingBuffers.h"
using ZShadeSandboxLighting::LightManager;
//==============================================================================================================================
//==============================================================================================================================
LightManager* LightManager::instance = 0;
int			  LightManager::mCount = 0;
int			  LightManager::mAmbientLightCount = 0;
int			  LightManager::mDirectionLightCount = 0;
int			  LightManager::mSpotLightCount = 0;
int			  LightManager::mPointLightCount = 0;
int			  LightManager::mCapsuleLightCount = 0;
//==============================================================================================================================
LightManager::LightManager(D3D* d3d)
:   m_D3DSystem(d3d)
{
	mLights.resize(MAX_LIGHTS);

	mLightBuffer = new ZShadeSandboxLighting::LightBuffer();
	mSunLightBuffer = new ZShadeSandboxLighting::SunLightBuffer();
	
	bToggleAmbientLights = true;
	bToggleDirectionalLights = true;
	bToggleSpotLights = true;
	bTogglePointLights = true;
	bToggleCapsuleLights = true;
}
//==============================================================================================================================
LightManager::~LightManager()
{
}
//==============================================================================================================================
void LightManager::NewInstance(D3D* d3d)
{
	if (instance == 0)
		instance = new LightManager(d3d);
}
//==============================================================================================================================
LightManager* LightManager::Instance()
{
	return instance;
}
//==============================================================================================================================
void LightManager::BuildLightingBuffers(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown, ZShadeSandboxLighting::SunLightBuffer buff)
{
	RebuildLightBuffer(ambientUp, ambientDown);
	RebuildSunBuffer(buff);
}
//==============================================================================================================================
void LightManager::RebuildLightBuffer(XMFLOAT3 ambientUp, XMFLOAT3 ambientDown)
{
	int ambientLightID = 0;
	int directionLightID = 0;
	int spotLightID = 0;
	int pointLightID = 0;
	int capsuleLightID = 0;
	
	// Get the lights in the scene
	for (int i = 0; i < ZShadeSandboxLighting::LightManager::mCount; i++)
	{
		ZShadeSandboxLighting::AmbientLight* ambientLight = GetAmbientLight(i);
		if (ambientLight != NULL)
		{
			ZShadeSandboxLighting::AmbientLightBuffer alb;
			alb.g_AmbientColor = ambientLight->AmbientColor();
			mLightBuffer->g_AmbientLight[ambientLightID++] = alb;
		}

		ZShadeSandboxLighting::DirectionalLight* dirLight = GetDirectionalLight(i);
		if (dirLight != NULL)
		{
			ZShadeSandboxLighting::DirectionalLightBuffer dlb;
			dlb.g_LightDirection = dirLight->Direction();
			dlb.g_DiffuseColor = dirLight->DiffuseColor();
			dlb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			mLightBuffer->g_DirectionalLight[directionLightID++] = dlb;
		}

		ZShadeSandboxLighting::SpotLight* spotLight = GetSpotLight(i);
		if (spotLight != NULL)
		{
			ZShadeSandboxLighting::SpotLightBuffer slb;
			slb.g_DiffuseColor = spotLight->DiffuseColor();
			slb.g_LightPosition = spotLight->Position();
			slb.g_LightRange = spotLight->Range();
			slb.g_SpotCosOuterCone = spotLight->SpotCosOuterCone();
			slb.g_SpotInnerConeReciprocal = spotLight->SpotInnerConeReciprocal();
			slb.g_CosineAngle = spotLight->SpotAngle();
			slb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			mLightBuffer->g_SpotLight[spotLightID++] = slb;
		}

		ZShadeSandboxLighting::PointLight* pointLight = GetPointLight(i);
		if (pointLight != NULL)
		{
			ZShadeSandboxLighting::PointLightBuffer plb;
			plb.g_LightPosition = pointLight->Position();
			plb.g_LightRange = pointLight->Range();
			plb.g_Attenuation = XMFLOAT3(0, 0, 0);
			plb.g_AmbientColor = XMFLOAT4(0, 0, 0, 0);
			plb.g_DiffuseColor = pointLight->DiffuseColor();
			mLightBuffer->g_PointLight[pointLightID++] = plb;
		}

		ZShadeSandboxLighting::CapsuleLight* capsuleLight = GetCapsuleLight(i);
		if (capsuleLight != NULL)
		{
			ZShadeSandboxLighting::CapsuleLightBuffer clb;
			clb.g_LightPosition = capsuleLight->Position();
			clb.g_DiffuseColor = capsuleLight->DiffuseColor();
			clb.g_AmbientColor = capsuleLight->AmbientColor();
			clb.g_LightRange = capsuleLight->Range();
			clb.g_LightLength = capsuleLight->LightLength();
			clb.g_CapsuleIntensity = capsuleLight->CapsuleIntensity();
			clb.g_CapsuleDirectionLength = capsuleLight->CapsuleDirectionLength();
			clb.g_LightDirection = capsuleLight->Direction();
			mLightBuffer->g_CapsuleLight[capsuleLightID++] = clb;
		}
	}
	
	mLightBuffer->g_AmbientDown = ambientDown;
	mLightBuffer->g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	mLightBuffer->g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	mLightBuffer->g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	mLightBuffer->g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	mLightBuffer->g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	mLightBuffer->g_AmbientUp = ambientUp;

	for (int i = 0; i < LightManager::mCount; i++)
	{
		mLights[i]->AddMeshLightBuffer(mLightBuffer);
	}
}
//==============================================================================================================================
void LightManager::RebuildSunBuffer(ZShadeSandboxLighting::SunLightBuffer buff)
{
	mSunLightBuffer->g_SunDir = buff.g_SunDir;
	mSunLightBuffer->g_SunDiffuseColor = buff.g_SunDiffuseColor;
	mSunLightBuffer->g_SunShineness = buff.g_SunShineness;
	mSunLightBuffer->g_EnableSun = buff.g_EnableSun;

	for (int i = 0; i < LightManager::mCount; i++)
	{
		mLights[i]->AddMeshLightBuffer(mSunLightBuffer);
	}
}
//==============================================================================================================================
void LightManager::ToggleAmbientLights(bool toggle)
{
	bToggleAmbientLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleDirectionalLights(bool toggle)
{
	bToggleDirectionalLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleSpotLights(bool toggle)
{
	bToggleSpotLights = toggle;
}
//==============================================================================================================================
void LightManager::TogglePointLights(bool toggle)
{
	bTogglePointLights = toggle;
}
//==============================================================================================================================
void LightManager::ToggleCapsuleLights(bool toggle)
{
	bToggleCapsuleLights = toggle;
}
//==============================================================================================================================
int LightManager::LightCount()
{
	return LightManager::mCount;
}
//==============================================================================================================================
int LightManager::TriangleCount() const
{
	int triCount = 0;
	
	for (int i = 0; i < LightManager::mCount; i++)
	{
		triCount += mLights[i]->TriangleCount();
	}
	
	return triCount;
}
//==============================================================================================================================
void LightManager::AddLight(ZShadeSandboxLighting::Light* light)
{
	light->BuildSphereMesh(m_D3DSystem);
	mLights[LightManager::mCount++] = light;

	switch (light->LightType())
	{
		case ZShadeSandboxLighting::ELightType::eAmbient:
			LightManager::mAmbientLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eDirectional:
			LightManager::mDirectionLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eSpot:
			LightManager::mSpotLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::ePoint:
			LightManager::mPointLightCount++;
		break;
		case ZShadeSandboxLighting::ELightType::eCapsule:
			LightManager::mCapsuleLightCount++;
		break;
	}
}
//==============================================================================================================================
void LightManager::AddAmbientLight(XMFLOAT4 ambientColor, XMFLOAT3 position)
{
	ZShadeSandboxLighting::AmbientLight* light = new ZShadeSandboxLighting::AmbientLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eAmbient;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mAmbientLightCount++;
}
//==============================================================================================================================
void LightManager::AddDirectionalLight(XMFLOAT4 diffuseColor, XMFLOAT4 ambientColor, XMFLOAT3 position, XMFLOAT3 direction)
{
	ZShadeSandboxLighting::DirectionalLight* light = new ZShadeSandboxLighting::DirectionalLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mDirectionLightCount++;
}
//==============================================================================================================================
void LightManager::AddSpotLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
,	float range
, 	float spotAngle
, 	XMFLOAT3 attenuation
)
{
	ZShadeSandboxLighting::SpotLight* light = new ZShadeSandboxLighting::SpotLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eSpot;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->SpotAngle() = spotAngle;
	light->Attenuation() = attenuation;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mSpotLightCount++;
}
//==============================================================================================================================
void LightManager::AddPointLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
,	float range
, 	XMFLOAT3 attenuation
)
{
	ZShadeSandboxLighting::PointLight* light = new ZShadeSandboxLighting::PointLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::ePoint;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->Attenuation() = attenuation;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mPointLightCount++;
}
//==============================================================================================================================
void LightManager::AddCapsuleLight
(	XMFLOAT4 diffuseColor
, 	XMFLOAT4 ambientColor
, 	XMFLOAT3 position
, 	XMFLOAT3 direction
,	float range
,	float lightLength
,	float capsuleIntensity
,	XMFLOAT3 capsuleDirectionLength
)
{
	ZShadeSandboxLighting::CapsuleLight* light = new ZShadeSandboxLighting::CapsuleLight();
	
	light->LightType() = ZShadeSandboxLighting::ELightType::eCapsule;
	light->DiffuseColor() = diffuseColor;
	light->AmbientColor() = ambientColor;
	light->Position() = position;
	light->Direction() = direction;
	light->Range() = range;
	light->LightLength() = lightLength;
	light->CapsuleIntensity() = capsuleIntensity;
	light->CapsuleDirectionLength() = capsuleDirectionLength;
	
	light->BuildSphereMesh(m_D3DSystem);
	
	mLights[LightManager::mCount++] = light;

	LightManager::mCapsuleLightCount++;
}
//==============================================================================================================================
void LightManager::RenderLightMesh(Camera* camera, LightCamera* lightcamera, bool toggleMesh, bool reflect, bool toggleWireframe, bool renderDeferred, XMFLOAT4 clipplane)
{
	// No lights to render
	if (!bToggleAmbientLights && !bToggleDirectionalLights && !bToggleSpotLights && !bTogglePointLights && !bToggleCapsuleLights)
		return;
	
	for (int i = 0; i < LightManager::mCount; i++)
	{
		int lightType = mLights[i]->LightType();
		
		if (!bToggleAmbientLights && lightType == ZShadeSandboxLighting::ELightType::eAmbient) continue;
		if (!bToggleDirectionalLights && lightType == ZShadeSandboxLighting::ELightType::eDirectional) continue;
		if (!bToggleSpotLights && lightType == ZShadeSandboxLighting::ELightType::eSpot) continue;
		if (!bTogglePointLights && lightType == ZShadeSandboxLighting::ELightType::ePoint) continue;
		if (!bToggleCapsuleLights && lightType == ZShadeSandboxLighting::ELightType::eCapsule) continue;
		
		mLights[i]->ToggleRenderDeferredMesh() = renderDeferred;
		mLights[i]->ToggleSphereMeshWireframe() = toggleWireframe;
		mLights[i]->ToggleSphereMesh() = toggleMesh;
		mLights[i]->RenderSphereMesh(camera, lightcamera, reflect, clipplane);
	}
}
//==============================================================================================================================
void LightManager::ClearLights()
{
	LightManager::mCount = 0;
	mLights.resize(0);
	
	// After it is cleared reinitialize
	mLights.resize(MAX_LIGHTS);
}
//==============================================================================================================================
void LightManager::DeleteLight(int i)
{
	if (mLights[i] != NULL)
	{
		// This will need updated and testing

		mLights.erase(mLights.begin() + i);

		//// Shuffle all the lights downward from the deleted position
		//for (int j = 0; j < MAX_LIGHTS - 1; j++)
		//{
		//	if (mLights[j] == NULL)
		//	{
		//		mLights[j] = mLights[j + 1]->Clone();
		//		
		//		mLights.erase(mLights.begin() + (j + 1));

		//		mLights[j + 1] = NULL;
		//	}
		//}
		
		LightManager::mCount--;
	}
}
//==============================================================================================================================
ZShadeSandboxLighting::AmbientLight* LightManager::GetAmbientLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eAmbient)
		{
			return (ZShadeSandboxLighting::AmbientLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::DirectionalLight* LightManager::GetDirectionalLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eDirectional)
		{
			return (ZShadeSandboxLighting::DirectionalLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::SpotLight* LightManager::GetSpotLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eSpot)
		{
			return (ZShadeSandboxLighting::SpotLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::PointLight* LightManager::GetPointLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::ePoint)
		{
			return (ZShadeSandboxLighting::PointLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================
ZShadeSandboxLighting::CapsuleLight* LightManager::GetCapsuleLight(int i)
{
	if (mLights[i] != NULL)
	{
		if (mLights[i]->LightType() == ZShadeSandboxLighting::ELightType::eCapsule)
		{
			return (ZShadeSandboxLighting::CapsuleLight*)mLights[i];
		}
	}

	return NULL;
}
//==============================================================================================================================