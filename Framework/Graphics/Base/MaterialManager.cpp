#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
MaterialManager* MaterialManager::m_instance = 0;
//===============================================================================================================================
//===============================================================================================================================
MaterialManager::MaterialManager()
{
}
//===============================================================================================================================
MaterialManager::~MaterialManager()
{
}
//===============================================================================================================================
MaterialManager* MaterialManager::Instance()
{
	if (m_instance == NULL)
		m_instance = new MaterialManager();
	return m_instance;
}
//===============================================================================================================================
void MaterialManager::AddDiffuse(D3D* d3d, string base_path, string name, string filename)
{
	ZShadeSandboxLighting::ShaderMaterial* mat = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	mat->AmbientColor() = XMFLOAT4(1, 1, 1, 1);
	mat->DiffuseColor() = XMFLOAT4(1, 1, 1, 1);
	mat->SpecularColor() = XMFLOAT4(1, 1, 1, 1);
	mat->SpecularPower() = 5.0f;
	mat->SpecularIntensity() = 0.3f;
	mat->EnableTransparency() = false;
	mat->EnableLighting() = true;
	mat->DetailBrightness() = 1.8f;
	mat->AddDiffuseTexture(base_path, filename);
	mat->AddDetailMapTexture(base_path, "detail001.dds");
	mat->AddNormalMapTexture(base_path, "lichen1_normal.dds");
	Add(mat);
}
//===============================================================================================================================
void MaterialManager::AddTransparentDiffuse(D3D* d3d, string base_path, string name, string filename, string alphaFilename, XMFLOAT3 alphaValue)
{
	ZShadeSandboxLighting::ShaderMaterial* mat = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	mat->AmbientColor() = XMFLOAT4(1, 1, 1, 1);
	mat->DiffuseColor() = XMFLOAT4(1, 1, 1, 1);
	mat->SpecularColor() = XMFLOAT4(1, 1, 1, 1);
	mat->SpecularPower() = 5.0f;
	mat->SpecularIntensity() = 0.3f;
	mat->EnableTransparency() = true;
	mat->AlphaToCoverageValue() = alphaValue;
	mat->EnableLighting() = true;
	mat->DetailBrightness() = 1.8f;
	mat->AddDiffuseTexture(base_path, filename);
	mat->AddDetailMapTexture(base_path, "detail001.dds");
	mat->AddNormalMapTexture(base_path, "lichen1_normal.dds");
	mat->AddAlphaMapTexture(base_path, alphaFilename);
	Add(mat);
}
//===============================================================================================================================
void MaterialManager::Add(ZShadeSandboxLighting::ShaderMaterial* material)
{
	map<string, ZShadeSandboxLighting::ShaderMaterial*>::iterator it = m_pMaterials.find(material->Name());
	if (it == m_pMaterials.end())
	{
		m_pMaterials.insert(make_pair(material->Name(), material));
	}
}
//===============================================================================================================================
ZShadeSandboxLighting::ShaderMaterial* MaterialManager::GetMaterial(string name)
{
	map<string, ZShadeSandboxLighting::ShaderMaterial*>::iterator it = m_pMaterials.find(name);
	if (it != m_pMaterials.end())
	{
		return (*it).second;
	}

	return NULL;
}
//===============================================================================================================================