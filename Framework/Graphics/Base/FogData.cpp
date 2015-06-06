#include "FogData.h"
//-----------------------------------------------------------------------------------------------------------------
ZShadeSandboxMesh::QuadMesh* FogData::fow_all_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_CSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JNW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JNE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JSW_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_JSE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_NNN_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_SSS_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_EEE_texture = NULL;
ZShadeSandboxMesh::QuadMesh* FogData::fow_WWW_texture = NULL;
//-----------------------------------------------------------------------------------------------------------------
D3D* FogData::m_D3DSystem = 0;
FogData* FogData::instance = 0;
//-----------------------------------------------------------------------------------------------------------------
signed char* FogData::m_fow_frame_table = 0;
//-----------------------------------------------------------------------------------------------------------------
FogData::FogData(D3D* d3d)
{
	FogData::m_D3DSystem = d3d;
}
//-----------------------------------------------------------------------------------------------------------------
FogData::FogData(const FogData& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
FogData::~FogData()
{
}
//-----------------------------------------------------------------------------------------------------------------
FogData* FogData::NewInstance(D3D* d3d)
{
	instance = new FogData(d3d);
	return instance;
}
//-----------------------------------------------------------------------------------------------------------------
FogData* FogData::Instance()
{
	return instance;
}
//-----------------------------------------------------------------------------------------------------------------