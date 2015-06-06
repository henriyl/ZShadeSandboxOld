#include "ShaderMaterial.h"
using ZShadeSandboxLighting::ShaderMaterial;
//==============================================================================================================================
//==============================================================================================================================
ShaderMaterial::ShaderMaterial(D3D* d3d, string name)
:   m_D3DSystem(d3d)
,   mAmbient(XMFLOAT4(0, 0, 0, 0))
,   mDiffuse(XMFLOAT4(0, 0, 0, 0))
,   mSpecular(XMFLOAT4(0, 0, 0, 0))
,   mSpecularPower(0)
,   mSpecularIntensity(0)
,   mName(name)
,   mEnableShadowMap(false)
,   mEnableSSAOMap(false)
,   mShadowMapTexture(0)
,   mSSAOTexture(0)
{
}
//==============================================================================================================================
ShaderMaterial::~ShaderMaterial()
{
	if (mMaterialTextures.size() > 0)
	{
		for (int i = 0; i < mMaterialTextures.size(); i++)
			mMaterialTextures[i]->texture->Shutdown();
		mMaterialTextures.clear();
	}
}
//==============================================================================================================================
int ShaderMaterial::GetMaterialTextureType(int i)
{
	if (MaterialExists(i))
		return mMaterialTextures[i]->materialTextureType;
	return EMaterialTextureType::eNone;
}
//==============================================================================================================================
Texture* ShaderMaterial::GetTexture(int i, int mtt)
{
	if (MaterialExists(i))
	{
		if (mMaterialTextures[i]->materialTextureType == mtt)
			return mMaterialTextures[i]->texture;
		else
			return NULL;
	}
	else
		return NULL;
}
//==============================================================================================================================
Texture* ShaderMaterial::GetTexture(int i)
{
	if (MaterialExists(i))
		return mMaterialTextures[i]->texture;
	else
		return NULL;
}
//==============================================================================================================================
bool ShaderMaterial::MaterialExists(int i)
{
	if (mMaterialTextures[i] != NULL)
		return true;
	return false;
}
//==============================================================================================================================
void ShaderMaterial::RemoveTexture(int i)
{
	if (MaterialExists(i))
	{
		mMaterialTextures[i]->texture->Shutdown();
		mMaterialTextures.erase(mMaterialTextures.begin() + i);
	}
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuse;
	matTexture->texture = tex;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialDetailMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDetail;
	matTexture->texture = tex;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialNormalMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eNormal;
	matTexture->texture = tex;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialBlendMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eBlend;
	matTexture->texture = tex;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::SetMaterialAlphaMapTexture(ID3D11ShaderResourceView* texSRV)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->SetSRV(texSRV);

	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAlpha;
	matTexture->texture = tex;

	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddDiffuseTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuse;
	matTexture->texture = tex;
	
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddDetailMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDetail;
	matTexture->texture = tex;
	
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddNormalMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eNormal;
	matTexture->texture = tex;
	
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddBlendMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eBlend;
	matTexture->texture = tex;
	
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::AddAlphaMapTexture(BetterString texturePath, BetterString textureName)
{
	Texture* tex = new Texture(m_D3DSystem);
	tex->Initialize(texturePath, textureName);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eAlpha;
	matTexture->texture = tex;
	
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================
void ShaderMaterial::CreateTexture2DArray(vector<string> textureFileNames)
{
	mDiffuseArrayTextureNames = textureFileNames;
	
	Texture* tex = new Texture(m_D3DSystem);
	tex->InitializeLayerMapArray(textureFileNames);
	
	MaterialTextureType* matTexture = new MaterialTextureType();
	matTexture->materialTextureType = EMaterialTextureType::eDiffuseArray;
	matTexture->texture = tex;
	
	//mMaterialTextures.insert(make_pair(name, matTexture));
	mMaterialTextures.push_back(matTexture);
}
//==============================================================================================================================