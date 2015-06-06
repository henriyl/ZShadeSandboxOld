#include "ImageSampleWindow.h"
#include "CGlobal.h"
using namespace DirectX;
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
ImageSampleWindow::ImageSampleWindow(HINSTANCE hInstance, HWND parent)
: m_running(false), m_created(false), m_escape(false)
{
	m_EngineOptions = new EngineOptions();
	m_EngineOptions->fullscreen = false;
	m_EngineOptions->m_d3dVersion = DIRECTX11;
	m_EngineOptions->m_DimType = ZSHADE_2D;
	m_EngineOptions->m_screenWidth = 64;
	m_EngineOptions->m_screenHeight = 64;
	
	bSpriteLoaded = false;

	m_Window = new Window();

	if (!m_Window->Init(hInstance, false, m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight,
		"Image Sample Window", "ImageSampleWindow", parent))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "ImageSampleWindow Creation Failed");
	}

	m_Window->SetUserMessageFunction(WM_PAINT, bind(mem_fn(&ImageSampleWindow::Paint), this, _1, _2, _3, _4));

	m_EngineOptions->hwnd = m_Window->GetHwnd();

	m_D3DSystem = new D3D();
	m_D3DSystem->Initialize11(m_EngineOptions);

	camera = new Camera(m_EngineOptions);
	camera->SetPosition(0.0f, 0.0f, -15.0f);
	camera->SetWorld(XMMatrixIdentity());
	camera->CreateOrtho(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, 0.1f, 1000.0f);

	textureShader = new TextureShader(m_D3DSystem);
	sprite = new Sprite(m_D3DSystem);
}
//================================================================================================================
void ImageSampleWindow::Init(HINSTANCE hInstance, HWND parent)
{
	m_running = false;
	m_created = false;
	m_escape = false;

	m_EngineOptions = new EngineOptions();
	m_EngineOptions->fullscreen = false;
	m_EngineOptions->m_d3dVersion = DIRECTX11;
	m_EngineOptions->m_DimType = ZSHADE_2D;
	m_EngineOptions->m_screenWidth = 64;
	m_EngineOptions->m_screenHeight = 64;

	bSpriteLoaded = false;

	m_Window = new Window();

	if (!m_Window->Init(hInstance, false, m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight,
		"Image Sample Window", "ImageSampleWindow", parent))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "ImageSampleWindow Creation Failed");
	}

	m_Window->SetUserMessageFunction(WM_PAINT, bind(mem_fn(&ImageSampleWindow::Paint), this, _1, _2, _3, _4));

	m_EngineOptions->hwnd = m_Window->GetHwnd();

	m_D3DSystem = new D3D();
	m_D3DSystem->Initialize11(m_EngineOptions);

	camera = new Camera(m_EngineOptions);
	camera->SetPosition(0.0f, 0.0f, -15.0f);
	camera->SetWorld(XMMatrixIdentity());
	camera->CreateOrtho(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, 0.1f, 1000.0f);

	textureShader = new TextureShader(m_D3DSystem);
	sprite = new Sprite(m_D3DSystem);
}
//================================================================================================================
void ImageSampleWindow::SetPosition(int x, int y)
{
	m_Window->SetWindowPos(x, y);
}
//================================================================================================================
bool ImageSampleWindow::InFocus()
{
	if (GetForegroundWindow() == m_Window->GetHwnd())
		return true;
	else
		return false;
}
//================================================================================================================
void ImageSampleWindow::Invalidate()
{
	m_Window->Invalidate();
}
//================================================================================================================
void ImageSampleWindow::LoadImageSample(string base_path, string imageName)
{
	BetterString str(base_path);
	str += "\\";
	str += imageName;

	this->imageName = str.toString();

	bSpriteLoaded = true;
}
//================================================================================================================
void ImageSampleWindow::Show(bool bShow)
{
	m_Window->Show(bShow);

	if (bShow)
		m_Window->Invalidate();
}
//================================================================================================================
LRESULT ImageSampleWindow::Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (bSpriteLoaded)
	{
		int size = 64;

		XMFLOAT2 pos = XMFLOAT2((m_EngineOptions->m_screenWidth / 2) - (size / 2), (m_EngineOptions->m_screenHeight / 2) - (size / 2));

		m_D3DSystem->GDIPlus()->DrawImage(hwnd, imageName.c_str(), pos.x, pos.y, 64, 64);
	}

	return 1;
}
//================================================================================================================