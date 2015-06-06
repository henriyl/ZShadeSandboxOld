//================================================================================================================================================================
// OceanHeightfield
//
//================================================================================================================================================================
// History
//
// -Created on 6/29/2014 by Dustin Watson
//================================================================================================================================================================
#ifndef __OCEANHEIGHTFIELD_H
#define __OCEANHEIGHTFIELD_H
//================================================================================================================================================================
//================================================================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "OceanParameters.h"

//================================================================================================================================================================
//================================================================================================================================================================
struct OceanHeightfield
{
	void Initialize();
	
	XMFLOAT2 wind_dir;
	float wave_amplitude;
	float wind_speed;
	float wind_dependency;
	float patch_size;
	float displacement_map_dim;

	void CreateHeightfield(ZShadeSandboxMath::XMMath2* out_h0, float* out_omega);
	float GaussRandom();
	float Phillips(ZShadeSandboxMath::XMMath2 K, ZShadeSandboxMath::XMMath2 windDir, float v, float a, float dir_depend);

	int heightfield_size;
	int heightfield_dim;
	int input_full_size;
	int input_half_size;
	int output_size;
	int zero_data_size;
	UINT float2_stride;

	ZShadeSandboxMath::XMMath2* h0_data;
	float* omega_data;
};
//================================================================================================================================================================
//================================================================================================================================================================
#endif//__OCEANHEIGHTFIELD_H