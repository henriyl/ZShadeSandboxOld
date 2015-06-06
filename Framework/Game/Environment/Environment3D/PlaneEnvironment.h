//===============================================================================================================================
// PlaneEnvironment
//
//===============================================================================================================================
// History
//
//		-Created on 6/15/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __PLANEENVIRONMENT_H
#define __PLANEENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "Environment3D.h"
#include "Vertex.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// Plane Environment
//
class PlaneEnvironment : public Environment3D
{
public:
	PlaneEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~PlaneEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();

private:
	//ID3D11Buffer* mVB;
	//ID3D11Buffer* mIB;
	//UINT mIndexCount;
	//ID3D11ShaderResourceView* mColorTexture;
	//ID3D11ShaderResourceView* mNormalTexture;
	//PosNormalTexTanShader* mShader;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__PLANEENVIRONMENT_H