//=====================================================================================
// ShaderMaterial.h
//
//==============================================================================================================================
// History
//
// -Created on 4/17/2014 by Dustin Watson
//=====================================================================================
#ifndef __SHADERMATERIAL_H
#define __SHADERMATERIAL_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "Texture.h"
#include "LightShadingBuffers.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
namespace EMaterialTextureType
{
	enum
	{
		eNone,
		eDiffuseArray,
		eDiffuse,
		eDetail,
		eNormal,
		eBlend,
		eAlpha
	};
}
//==============================================================================================================================
//==============================================================================================================================
class ShaderMaterial
{
	struct MaterialTextureType
	{
		int materialTextureType;
		Texture* texture;
	};

public:
	
	ShaderMaterial(D3D* d3d, string name);
	
	// Release all textures
	~ShaderMaterial();
	
	void AddDiffuseTexture(BetterString texturePath, BetterString textureName);
	void AddDetailMapTexture(BetterString texturePath, BetterString textureName);
	void AddNormalMapTexture(BetterString texturePath, BetterString textureName);
	void AddBlendMapTexture(BetterString texturePath, BetterString textureName);
	void AddAlphaMapTexture(BetterString texturePath, BetterString textureName);
	
	// Creates a Texture2DArray SRV
	void CreateTexture2DArray(vector<string> textureFileNames);
	
	void SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialDetailMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialNormalMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialBlendMapTexture(ID3D11ShaderResourceView* texSRV);
	void SetMaterialAlphaMapTexture(ID3D11ShaderResourceView* texSRV);

	void RemoveTexture(int i);

public:
	
	void SetDiffuseArrayTextureNames(vector<string> names) { mDiffuseArrayTextureNames = names; }
	vector<string> GetDiffuseArrayTextureNames() { return mDiffuseArrayTextureNames; }
	
	vector<MaterialTextureType*> Textures() const 	{ return mMaterialTextures; }
	Texture* GetTexture(int i, int mtt);
	Texture* GetTexture(int i);
	int GetMaterialTextureType(int i);
	bool MaterialExists(int i);
	int TextureCount()						{ return mMaterialTextures.size(); }
	string& Name()							{ return mName; }
	XMFLOAT4& AmbientColor() 				{ return mAmbient; }
	XMFLOAT4& DiffuseColor() 				{ return mDiffuse; }
	XMFLOAT4& SpecularColor() 				{ return mSpecular; }
	float& SpecularPower() 					{ return mSpecularPower; }
	float& SpecularIntensity() 				{ return mSpecularIntensity; }
	float& Emissivity()		 				{ return fEmissivity; }
	float& Reflectivity()	 				{ return fReflectivity; }
	float& Transmissivity()	 				{ return fTransmissivity; }
	bool& EnableTransparency()				{ return mEnableTransparency; }
	bool& EnableLighting()					{ return mEnableLighting; }
	float& DetailBrightness()				{ return mDetailBrightness; }
	XMFLOAT3& AlphaToCoverageValue()		{ return mAlphaToCoverageValue; }
	bool& EnableShadowMap()					{ return mEnableShadowMap; }
	bool& EnableSSAOMap()					{ return mEnableSSAOMap; }
	
	string Name() const						{ return mName; }
	XMFLOAT4 AmbientColor() const 			{ return mAmbient; }
	XMFLOAT4 DiffuseColor() const 			{ return mDiffuse; }
	XMFLOAT4 SpecularColor() const 			{ return mSpecular; }
	float SpecularPower() const 			{ return mSpecularPower; }
	float SpecularIntensity() const 		{ return mSpecularIntensity; }
	float Emissivity() const				{ return fEmissivity; }
	float Reflectivity() const				{ return fReflectivity; }
	float Transmissivity() const			{ return fTransmissivity; }
	bool EnableTransparency() const			{ return mEnableTransparency; }
	bool EnableLighting() const				{ return mEnableLighting; }
	float DetailBrightness() const			{ return mDetailBrightness; }
	XMFLOAT3 AlphaToCoverageValue() const	{ return mAlphaToCoverageValue; }
	bool EnableShadowMap() const			{ return mEnableShadowMap; }
	bool EnableSSAOMap() const				{ return mEnableSSAOMap; }
	
	void SetLightBuffer(ZShadeSandboxLighting::LightBuffer* lb) { mLightBuffer = lb; }
	void SetLightBuffer(ZShadeSandboxLighting::SunLightBuffer* slb) { mSunLightBuffer = slb; }
	
	ZShadeSandboxLighting::LightBuffer* GetLightBuffer() { return mLightBuffer; }
	ZShadeSandboxLighting::SunLightBuffer* GetSunLightBuffer() { return mSunLightBuffer; }
	
	void SetShadowMapTexture(ID3D11ShaderResourceView* srv) { mShadowMapTexture = srv; }
	void SetSSAOTexture(ID3D11ShaderResourceView* srv) { mSSAOTexture = srv; }

	ID3D11ShaderResourceView* ShadowMapTexture() { return mShadowMapTexture; }
	ID3D11ShaderResourceView* SSAOTexture() { return mSSAOTexture; }

private:
	
	D3D* m_D3DSystem;
	
	string mName;
	
	vector<MaterialTextureType*> mMaterialTextures;
	vector<string> mDiffuseArrayTextureNames;

	ID3D11ShaderResourceView* mShadowMapTexture;
	ID3D11ShaderResourceView* mSSAOTexture;
	
	ZShadeSandboxLighting::LightBuffer* mLightBuffer;
	ZShadeSandboxLighting::SunLightBuffer* mSunLightBuffer;
	
	XMFLOAT4 mAmbient;
	XMFLOAT4 mDiffuse;
	XMFLOAT4 mSpecular;
	
	// Used with radiosity
	// Effectiveness in emitting light
	float fEmissivity;
	// The ratio of the energy of a wave reflected from the material to the energy possessed by the wave striking the material
	float fReflectivity;
	// Measure of the ability for this material to transmit light
	float fTransmissivity;
	// Used with radiosity
	
	float mSpecularPower;
	float mSpecularIntensity;
	
	bool mEnableShadowMap;
	bool mEnableSSAOMap;
	bool mEnableTransparency;
	bool mEnableLighting;
	float mDetailBrightness;
	XMFLOAT3 mAlphaToCoverageValue; // Value that clips pixel during alpha blending
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__SHADERMATERIAL_H