//===============================================================================================================================
// OBJMeshSurfaceMaterial.h
//
//===============================================================================================================================
// History
//
// -Created on 4/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESHSURFACEMATERIAL_H
#define __OBJMESHSURFACEMATERIAL_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#include <fstream>
#include <istream>
#include "ZMath.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
struct OBJMeshSurfaceMaterial
{
	std::string 				sMaterialName;
	std::string 				sDiffuseTextureName;
	std::string 				sAmbientTextureName;
	std::string 				sSpecularTextureName;
	std::string 				sAlphaTextureName;
	std::string 				sNormalMapTextureName;
	std::string 				sEmissiveTextureName;
	
	ID3D11ShaderResourceView* 	diffuseSRV;
	ID3D11ShaderResourceView* 	ambientSRV;
	ID3D11ShaderResourceView* 	specularSRV;
	ID3D11ShaderResourceView* 	alphaSRV;
	ID3D11ShaderResourceView* 	normalMapSRV;
	ID3D11ShaderResourceView* 	emissiveSRV;
	
	// diffuse reflectivity
	XMFLOAT3					vDiffuseColor;
	// ambient reflectivity
	XMFLOAT3					vAmbientColor;
	// specular reflectivity
	XMFLOAT3					vSpecularColor;
	// emissive reflectivity
	XMFLOAT3					vEmissiveColor;
	
	// Any light passing through the object is filtered by the transmission filter,
	// which only allows the specific colors to pass through.
	// For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue. 
	XMFLOAT3					vTransmissionFilter;
	
	// amount this material dissolves into the background. A factor of 1.0 is fully opaque.
	// This is the default when a new material is created. A factor of 0.0 is fully dissolved (completely transparent).
	float						fAlpha;
	
	// specular exponent for the current material. This defines the focus of the specular highlight. 
	float 						fSpecularPower;
	float						fSpecularIntensity;
	
	// Specifies the optical density for the surface. This is also known as index of refraction. 
	// "optical_density" is the value for the optical density. The values can range from 0.001 to 10.
	// A value of 1.0 means that light does not bend as it passes through an object.
	// Increasing the optical_density increases the amount of bending. Glass has an index of refraction of about 1.5.
	// Values of less than 1.0 produce bizarre results and are not recommended. 
	float						fRefractionIndex;
	
	/*
		Illumination models are mathematical equations that represent various material lighting and shading effects.
		
		0	Color on and Ambient off 
		1	Color on and Ambient on 
		2	Highlight on 
		3	Reflection on and Ray trace on 
		4	Transparency: Glass on 
		Reflection: Ray trace on 
		5	Reflection: Fresnel on and Ray trace on 
		6	Transparency: Refraction on 
		Reflection: Fresnel off and Ray trace on 
		7	Transparency: Refraction on 
		Reflection: Fresnel on and Ray trace on 
		8	Reflection on and Ray trace off 
		9	Transparency: Glass on 
		Reflection: Ray trace off 
		10	Casts shadows onto invisible surfaces
	*/
	int							iIlluminationModel;
	
	bool 						bSpecularToggle;
	bool						bHasDiffuseTexture;
	bool						bHasAmbientTexture;
	bool						bHasSpecularTexture;
	bool						bHasEmissiveTexture;
	bool						bHasNormalMap;
	bool						bHasTransparency;
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJMESHSURFACEMATERIAL_H