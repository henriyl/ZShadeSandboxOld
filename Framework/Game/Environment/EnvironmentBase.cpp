#include "EnvironmentBase.h"
#include <functional>
#include "Window.h"
#include "ZShadeResource.h"
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
EnvironmentBase::EnvironmentBase(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name)
{
	InitBase(engineOptions, base_window_name, render_window_name);
}
//================================================================================================================
bool EnvironmentBase::InitBase(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name)
{
	InitCommonControls();
	
	//m_BaseWindow = iBaseWin;
	//m_RenderWindow = iRenderWin;
	//m_HInstance = hInstance;
	
	m_EngineOptions = engineOptions;
	
	if (m_EngineOptions->m_inEditor)
	{
		m_BaseEditWindow = new SplitterWindow();

		if (!m_BaseEditWindow->Init(m_EngineOptions->hinstance, m_EngineOptions->fullscreen, m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight,
			"ZShadeSandbox", base_window_name, IDI_ICON, IDC_TARGET, false, false))
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "Base Editor Window Creation Failed");
			return false;
		}

		// The left window is for DirectX rendering
		m_EngineOptions->hwnd = m_BaseEditWindow->GetHwndLeft();

		// The right window is for the tool window
		m_EngineOptions->toolHwnd = m_BaseEditWindow->GetHwndRight();

		// Show all the windows
		m_BaseEditWindow->Show();

		m_BaseEditWindow->CreateMenuResource(IDR_MENU);

		// Set the message functions for the DirectX window
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_SIZE, bind(mem_fn(&EnvironmentBase::WindowResized), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_LBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_MBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_RBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_LBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_MBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_RBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_BaseEditWindow->SetUserMessageFunctionLeft(WM_MOUSEMOVE, bind(mem_fn(&EnvironmentBase::WindowMouseMove), this, _1, _2, _3, _4));
	}
	else
	{
		m_BaseWindow = new Window();

		if (!m_BaseWindow->Init(m_EngineOptions->hinstance, m_EngineOptions->fullscreen, m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight,
			"ZShadeSandbox", base_window_name, NULL, IDI_ICON, IDC_TARGET, false, false))
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "Base Window Creation Failed");
			return false;
		}

		m_RenderWindow = new Window();

		int rSize = m_EngineOptions->m_screenWidth;

		//if (inEditor)
		{
			//rSize = rSize / 2;// +100;
		}

		if (!m_RenderWindow->Init(m_EngineOptions->hinstance, m_EngineOptions->fullscreen, rSize, m_EngineOptions->m_screenHeight,
			"ZShadeSandbox", render_window_name, m_BaseWindow->GetHwnd(), IDI_ICON, IDC_TARGET, false, false))
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "Render Window Creation Failed");
			return false;
		}

		m_EngineOptions->hwnd = m_RenderWindow->GetHwnd();

		m_BaseWindow->Show();
		m_RenderWindow->Show();

		m_RenderWindow->SetUserMessageFunction(WM_SIZE, bind(mem_fn(&EnvironmentBase::WindowResized), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_LBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_MBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_RBUTTONDOWN, bind(mem_fn(&EnvironmentBase::WindowMouseDown), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_LBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_MBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_RBUTTONUP, bind(mem_fn(&EnvironmentBase::WindowMouseUp), this, _1, _2, _3, _4));
		m_RenderWindow->SetUserMessageFunction(WM_MOUSEMOVE, bind(mem_fn(&EnvironmentBase::WindowMouseMove), this, _1, _2, _3, _4));
	}

	return true;
}
//================================================================================================================
EnvironmentBase::~EnvironmentBase()
{
}
//================================================================================================================
void EnvironmentBase::GoFullscreen(bool bEnable)
{
	m_EngineOptions->fullscreen = bEnable;
	m_RenderWindow->SetFullscreen(bEnable);
	m_BaseWindow->SetFullscreen(bEnable);
	m_D3DSystem->SetFullscreen(bEnable);
}
//================================================================================================================
void EnvironmentBase::SetGameDirectory(GameDirectory2D* gd)
{
	m_GameDirectory2D = gd;
	m_EngineOptions->m_GD2D = gd;
}
//================================================================================================================
void EnvironmentBase::SetGameDirectory(GameDirectory3D* gd)
{
	m_GameDirectory3D = gd;
	m_EngineOptions->m_GD3D = gd;
}
//===============================================================================================================================
void EnvironmentBase::BeforeReset()
{
}
//================================================================================================================
void EnvironmentBase::AfterReset()
{
	//int bbWidth, bbHeight;
	//m_D3DSystem->GetBackbufferSize(bbWidth, bbHeight);

	//float aspect = static_cast<float>(bbWidth) / bbHeight;
    
	//m_CameraSystem->AfterReset( aspect );

    //m_D3DSystem->InitializePostProcessingTargets();
}
//================================================================================================================
void EnvironmentBase::OnWindowResized(WPARAM wParam)
{
	/*if(!m_D3DSystem->GetEngineOptions()->fullscreen && wParam != SIZE_MINIMIZED)
    {
        int width, height;
        m_RenderWindow->GetClientArea(width, height);
		
		int bbWidth, bbHeight;
		m_D3DSystem->GetBackbufferSize(bbWidth, bbHeight);
		
        if(width != bbWidth || height != bbHeight)
        {
            BeforeReset();
			
			m_D3DSystem->SetBackbufferSize(width, height);
			m_D3DSystem->ResizeDevice();
			
            AfterReset();
        }
    }*/
}
//================================================================================================================
void EnvironmentBase::OnMouseDown(WPARAM btnState, int x, int y)
{
}
//================================================================================================================
void EnvironmentBase::OnMouseUp(WPARAM btnState, int x, int y)
{
}
//================================================================================================================
void EnvironmentBase::OnMouseMove(WPARAM btnState, int x, int y)
{
}
//================================================================================================================
LRESULT EnvironmentBase::WindowResized(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OnWindowResized(wParam);
	
    return 0;
}
//================================================================================================================
LRESULT EnvironmentBase::WindowMouseDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	return 0;
}
//================================================================================================================
LRESULT EnvironmentBase::WindowMouseUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	return 0;
}
//================================================================================================================
LRESULT EnvironmentBase::WindowMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	
	return 0;
}
//================================================================================================================