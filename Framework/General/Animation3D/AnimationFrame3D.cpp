#include "AnimationFrame3D.h"
//==================================================================================================================================
//==================================================================================================================================
AnimationFrame3D::AnimationFrame3D(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================