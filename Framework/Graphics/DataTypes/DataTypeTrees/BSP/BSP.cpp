#include "BSP.h"
//==================================================================================================================================
//==================================================================================================================================
BSP::BSP(D3D* d3d)
:   m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
}
//==================================================================================================================================