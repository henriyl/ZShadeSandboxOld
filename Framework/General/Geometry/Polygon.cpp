#include "Polygon.h"
using ZShadeSandboxMath::Polygon;
//==================================================================================================================================
//==================================================================================================================================
Polygon::Polygon(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
}
//==================================================================================================================================
void Polygon::AddVertex(XMFLOAT3 v)
{
	m_points.push_back(v);
}
//==================================================================================================================================
XMFLOAT3 Polygon::GetVertex(int id)
{
	return m_points[id];
}
//==================================================================================================================================
int Polygon::GetNumVertices()
{
	return m_points.size();
}
//==================================================================================================================================