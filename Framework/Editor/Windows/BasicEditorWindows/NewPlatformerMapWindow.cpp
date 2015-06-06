#include <windowsx.h>
#include "NewPlatformerMapWindow.h"
#include "TextureManager.h"
#include "CGlobal.h"
#include <sstream>
#include "Convert.h"
#include "ZShadeMessageCenter.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
NewMapWindowPlatformer::NewMapWindowPlatformer(MapEditorSystem2D* mapEditorSystem, HINSTANCE hInstance, D3D* d3d)
:   m_running(false)
,   m_xSize(0)
,   m_ySize(0)
,	m_created(false)
,   m_windowWidth(450)
,   m_windowHeight(250)
,   m_mapEditorSystem2D(mapEditorSystem)
,   m_WorldName("")
,   m_mapCreated(false)
,   m_D3DSystem(d3d)
{
	m_Window = new Window();
	
	if (!m_Window->Init(hInstance, false, m_windowWidth, m_windowHeight, "New Map Window Platformer", "NewMapWindowPlatformer", NULL))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "NewMapWindowPlatformer Creation Failed");
	}
	
	m_Window->SetUserMessageFunction(WM_COMMAND, bind(mem_fn(&NewMapWindowPlatformer::Command), this, _1, _2, _3, _4));
	m_Window->SetUserMessageFunction(WM_PAINT, bind(mem_fn(&NewMapWindowPlatformer::Paint), this, _1, _2, _3, _4));
	
	int x = 30;
	int y = 30;
	
	//y = 70;
	m_mapWidthLabel.Init(m_Window->GetHwnd(), "Width", x, y, 100, 20, PLAT_MAP_WIDTH_TEXT);
	m_MapWidthTextbox.Init(m_Window->GetHwnd(), x + 110, y, 100, 20, PLAT_MAP_WIDTH_TEXTBOX);
	y += 30;
	m_mapHeightLabel.Init(m_Window->GetHwnd(), "Height", x, y, 100, 20, PLAT_MAP_HEIGHT_TEXT);
	m_MapHeightTextbox.Init(m_Window->GetHwnd(), x + 110, y, 100, 20, PLAT_MAP_HEIGHT_TEXTBOX);
	
	y += 30;
	m_worldNameLabel.Init(m_Window->GetHwnd(), "World Name", x, y + 5, 50, 12, PLAT_WORLD_NAME_TEXT);
	m_worldNameComboBox.Init(m_Window->GetHwnd(), x + 60, y, 80, 500, PLAT_WORLD_NAME_CB);
	
	y += 35;
	m_MapNameLabel.Init(m_Window->GetHwnd(), "Map Name", x, y, 60, 12, PLAT_MAP_NAME_TEXT);
	m_MapNameTextbox.Init(m_Window->GetHwnd(), x + 60, y, 100, 20, PLAT_MAP_NAME);

	x = 30;
	m_createButton.Init(m_Window->GetHwnd(), "Create", x, 160, 60, 30, PLAT_CREATE_BUTTON);
	m_closeButton.Init(m_Window->GetHwnd(), "Close", x + 70, 160, 60, 30, PLAT_CLOSE_BUTTON);
}
//================================================================================================================
NewMapWindowPlatformer::NewMapWindowPlatformer(const NewMapWindowPlatformer& other)
{
}
//================================================================================================================
NewMapWindowPlatformer::~NewMapWindowPlatformer()
{
}
//================================================================================================================
void NewMapWindowPlatformer::AddWorldName(string name)
{
	m_createdWorlds.push_back(name);

	//m_worldNameComboBox.AddItem(name);
	//m_worldNameComboBox.SetDisplayItem(name, 0);

	// Update the world list in the combo box
	m_worldNameComboBox.Reset();

	string first_name = m_createdWorlds[0];
	for (int i = 0; i < m_createdWorlds.size(); i++)
	{
		string world_name = m_createdWorlds[i];

		m_worldNameComboBox.AddItem(world_name);
	}

	m_worldNameComboBox.SetDisplayItem(first_name, 0);
}
//================================================================================================================
void NewMapWindowPlatformer::SetPosition(int x, int y)
{
	m_Window->SetWindowPos(x, y);
}
//================================================================================================================
void NewMapWindowPlatformer::Invalidate()
{
	m_Window->Invalidate();
}
//================================================================================================================
/*void NewMapWindowPlatformer::CreateGUIComponents(HWND hwnd)
{
	//Get the background image path
	/*
	string path = CUtility::FindInitialPath();
	path.append("\\Textures\\NewMapBackground.bmp");
	m_backgroundImage = (HBITMAP)LoadImage(NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_backgroundImage == NULL)
	{
		// LoadImage faled so get extended error information.
		DWORD dwError = ::GetLastError();

		MessageBox(m_hwnd, "LoadImage Failed", "NewMapWindow Error", MB_OK);
	}
	*
	
	int x = 30;
	int y = 30;

	m_createButton = new WinButton(hwnd, "Create", x, 210, 60, 30, PLAT_CREATE_BUTTON);
	m_closeButton = new WinButton(hwnd, "Close", x + 70, 210, 60, 30, PLAT_CLOSE_BUTTON);

	y = 70;
	m_mapWidthLabel.Init(hwnd, "Width", x, y, 100, 20, PLAT_MAP_WIDTH_TEXT);
	m_MapWidthTextbox.Init(hwnd, x + 110, y, 100, 20, PLAT_MAP_WIDTH_TEXTBOX);
	y += 30;
	m_mapHeightLabel.Init(hwnd, "Height", x, y, 100, 20, PLAT_MAP_HEIGHT_TEXT);
	m_MapHeightTextbox.Init(hwnd, x + 110, y, 100, 20, PLAT_MAP_HEIGHT_TEXTBOX);

	y += 30;
	m_worldNameLabel = new WinText(hwnd, "World Name", x, y + 5, 50, 12, PLAT_WORLD_NAME_TEXT);
	m_worldNameComboBox = new WinComboBox(hwnd, x + 60, y, 80, 500, PLAT_WORLD_NAME_CB);

	y += 35;
	m_MapNameLabel = new WinText(hwnd, "Map Name", x, y, 60, 12, PLAT_MAP_NAME_TEXT);
	m_MapNameTextbox = new WinTextbox(hwnd, x + 60, y, 100, 20, PLAT_MAP_NAME);
}*/
//--------------------------------------------------------------------------------------------------------------------
/*void NewMapWindowPlatformer::BltBackground(HDC hdc)
{
	HDC hdcLocal = CreateCompatibleDC(hdc);

	if (hdcLocal == NULL)
	{
		MessageBox(m_hwnd, "CreateCompatibleDC Failed", "NewMapWindow Error", MB_OK);
		return;
	}

	BITMAP g_background;

	int r = GetObject(reinterpret_cast<HGDIOBJ>(m_backgroundImage), sizeof(BITMAP), reinterpret_cast<LPVOID>(&g_background));

	if (!r)
	{
		MessageBox(m_hwnd, "GetObject Failed", "NewMapWindow Error", MB_OK);
		return;
	}

	HBITMAP g_oldBackground = (HBITMAP)SelectObject(hdcLocal, m_backgroundImage);

	if (g_oldBackground == NULL)
	{
		MessageBox(m_hwnd, "SelectObject Failed", "NewMapWindow Error", MB_OK);
		return;
	}

	BOOL result = BitBlt(hdc, 0, 0, g_background.bmWidth, g_background.bmHeight, hdcLocal, 0, 0, SRCCOPY);

	if (!result)
	{
		MessageBox(m_hwnd, "BitBlt Failed", "NewMapWindow Error", MB_OK);
		return;
	}

	SelectObject(hdcLocal, g_oldBackground);
	DeleteDC(hdcLocal);
}*/
//--------------------------------------------------------------------------------------------------------------------
/*void NewMapWindowPlatformer::ProcessUpdates(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	m_worldNameComboBox->UpdateCommand(wParam, lParam);

	switch (LOWORD(wParam))
	{
		case CREATE_BUTTON:
		{
			//Get all the attributes
			bool hasMapWidth = false;
			bool hasMapHeight = false;

			//Get the width and height of the map
			if (m_MapWidthTextbox.GetText() == "")
				ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map width !!!", "Create Platformer Map");
			else
				hasMapWidth = true;

			if (m_MapHeightTextbox.GetText() == "")
				ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map height !!!", "Create Platformer Map");
			else
				hasMapHeight = true;

			if (hasMapWidth && hasMapHeight)
			{
				m_xSize = atoi(m_MapWidthTextbox.GetText().c_str());
				m_ySize = atoi(m_MapHeightTextbox.GetText().c_str());

				// Get the name of the world
				string wstr = m_worldNameComboBox->GetSelectedItem();
				if (wstr != "")
				{
					m_WorldName = wstr;
				}
				else
				{
					m_WorldName = "";
				}

				m_escape = true;

				if (m_MapNameTextbox->GetText() == "")
					ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map name !!!", "Create Map");
				else
				{
					if (m_WorldName == "")
						ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a world name !!!", "Create Map");
					else
					{
						m_mapCreated = true;

						m_mapEditorSystem2D->NewMapPlatformer(m_WorldName, m_MapNameTextbox->GetText(),
							m_xSize, m_ySize, true, m_hwndProgressBar);
					}
				}
			}
		}
		break;
		case CLOSE_BUTTON:
		{
			m_escape = true;
		}
		break;
	}
}*/
//--------------------------------------------------------------------------------------------------------------------
/*bool NewMapWindowPlatformer::OnCreate()
{
	m_newPlatformerMapInstance = this;

	WNDCLASSEX winclass_ex;

	ZeroMemory(&winclass_ex, sizeof(WNDCLASSEX));

	winclass_ex.cbSize = sizeof(WNDCLASSEX);
	winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	winclass_ex.lpfnWndProc = (WNDPROC)NewPlatformerMapWndProc;
	winclass_ex.cbClsExtra = 0;
	winclass_ex.cbWndExtra = 0;
	winclass_ex.hInstance = GetModuleHandle(NULL);
	winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass_ex.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );//(HBRUSH)(COLOR_WINDOW + 1);
	winclass_ex.lpszMenuName = NULL;
	winclass_ex.lpszClassName = "NewPlatformerMapClass";
	winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass_ex))
	{
		MessageBox(NULL, "Registration Failed", "Error", MB_OK);
		return false;
	}

	int size_x = m_windowWidth;
	int size_y = m_windowHeight;

	RECT wr = {0, 0, size_x, size_y};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPED, FALSE);    // adjust the size

	int w = wr.right - wr.left;
	int h = wr.bottom - wr.top;

	m_hwnd = CreateWindowEx(
		WS_OVERLAPPED,// | WS_EX_TOPMOST,
		"NewPlatformerMapClass",
		"New Platformer Map",
		WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		NULL, NULL,
		GetModuleHandle(NULL),
		NULL);

	if (m_hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed", "Error", MB_OK);
		return false;
	}
}*/
//================================================================================================================
bool NewMapWindowPlatformer::InFocus()
{
	if (GetForegroundWindow() == m_Window->GetHwnd())
		return true;
	else
		return false;
}
//================================================================================================================
int NewMapWindowPlatformer::Run()
{
	while(m_Window->IsAlive())
	{
		if (!m_Window->WinIsMinimized())
		{
			// Do Something
			
			if (m_escape) break;
		}

		m_Window->Run();
	}
	
	Show(false);
	return 1;
}
//================================================================================================================
void NewMapWindowPlatformer::Show(bool show)
{
	m_Window->Show(show);

	if (show)
	{
		m_created = false;
		m_running = true;
		m_escape = false;
		//SetFocus(m_hwnd);
		
		m_Window->Invalidate();
	}
	else
	{
		m_created = true;
		m_running = false;
		m_escape = true;
	}
}
//================================================================================================================
LRESULT NewMapWindowPlatformer::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_worldNameComboBox.UpdateCommand(wParam, lParam);

	switch (LOWORD(wParam))
	{
		case PLAT_CREATE_BUTTON:
		{
			//Get all the attributes
			bool hasMapWidth = false;
			bool hasMapHeight = false;

			//Get the width and height of the map
			if (m_MapWidthTextbox.GetText() == "")
				ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map width !!!", "Create Platformer Map");
			else
				hasMapWidth = true;

			if (m_MapHeightTextbox.GetText() == "")
				ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map height !!!", "Create Platformer Map");
			else
				hasMapHeight = true;

			if (hasMapWidth && hasMapHeight)
			{
				m_xSize = atoi(m_MapWidthTextbox.GetText().c_str());
				m_ySize = atoi(m_MapHeightTextbox.GetText().c_str());

				// Get the name of the world
				string wstr = m_worldNameComboBox.GetSelectedItem();
				if (wstr != "")
				{
					m_WorldName = wstr;
				}
				else
				{
					m_WorldName = "";
				}

				m_escape = true;

				if (m_MapNameTextbox.GetText() == "")
					ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a map name !!!", "Create Map");
				else
				{
					if (m_WorldName == "")
						ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a world name !!!", "Create Map");
					else
					{
						m_mapCreated = true;

						m_mapEditorSystem2D->NewMapPlatformer(m_WorldName, m_MapNameTextbox.GetText(),
							m_xSize, m_ySize, true, m_hwndProgressBar);
					}
				}
			}
		}
		break;
		case PLAT_CLOSE_BUTTON:
		{
			m_escape = true;
		}
		break;
	}
	
	return 1;
}
//================================================================================================================
LRESULT NewMapWindowPlatformer::Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_D3DSystem->GDIPlus()->DrawRectangle(hwnd, RGBColor::Color::SystemColor(), 0, 0, m_windowWidth, m_windowHeight);
	
	return 1;
}
//================================================================================================================
/*LRESULT CALLBACK NewMapWindowPlatformer::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			CreateGUIComponents(hwnd);
		}
		break;
		case WM_COMMAND:
		{
			ProcessUpdates(hwnd, wParam, lParam);
		}
		break;
		case WM_PAINT:
		{
			CGlobal::ClientResize(hwnd, m_windowWidth, m_windowHeight);

			/*PAINTSTRUCT ps;
			m_winDC = BeginPaint(hwnd, &ps);
			BltBackground(m_winDC);
			EndPaint(hwnd, &ps);*
		}
		break;
		case WM_CTLCOLORSTATIC:
		{
			int iControlId = GetWindowLong((HWND) lParam, GWL_ID);
			switch(iControlId)
			{
				case PLAT_MAP_WIDTH_TEXT:
				case PLAT_MAP_HEIGHT_TEXT:
				case PLAT_MAP_NAME_TEXT:
				case PLAT_WORLD_NAME_TEXT:
				{
					SetTextColor((HDC)wParam, RGB(255,255,255));
					SetBkColor((HDC)wParam, RGB(255,0,0));
					SetBkMode((HDC)wParam, TRANSPARENT);
					return (LRESULT)((HBRUSH)GetStockObject( NULL_BRUSH ));
				}
				break;
			}
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK NewPlatformerMapWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	return m_newPlatformerMapInstance->WinProc(hwnd, msg, wParam, lParam);
}*/
//--------------------------------------------------------------------------------------------------------------------