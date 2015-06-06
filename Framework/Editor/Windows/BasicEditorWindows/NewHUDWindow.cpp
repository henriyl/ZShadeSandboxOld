#include "NewHUDWindow.h"
#include <sstream>
#include "Convert.h"
#include "ZShadeMessageCenter.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
NewHUDWindow::NewHUDWindow(HUDEditorSystem* hudEditorSystem, HINSTANCE hInstance, D3D* d3d)
:   m_running(false)
,   m_created(false)
,   m_escape(false)
,   m_windowWidth(450)
,   m_windowHeight(250)
,   m_hudWidth(800)
,   m_hudHeight(600)
,   m_HUDEditorSystem(hudEditorSystem)
,   m_D3DSystem(d3d)
{
	m_Window = new Window();
	
	if (!m_Window->Init(hInstance, false, m_windowWidth, m_windowHeight, "New HUD Window", "NewHUDWindow", NULL))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "NewHUDWindow Creation Failed");
	}
	
	m_Window->SetUserMessageFunction(WM_COMMAND, bind(mem_fn(&NewHUDWindow::Command), this, _1, _2, _3, _4));
	m_Window->SetUserMessageFunction(WM_PAINT, bind(mem_fn(&NewHUDWindow::Paint), this, _1, _2, _3, _4));
	
#pragma region "GUI"
	int x = 10;
	int y = 30;

	int textWidth = 100;
	int textboxXDiff = textWidth + 10;
	int textboxWidth = 100;

	//m_menuNameText.Create(m_Window->GetHwnd(), "Menu Name", x, y, textWidth, 16, MENU_NAME_TEXT);
	//m_menuNameTextbox.Init(m_Window->GetHwnd(), x + textboxXDiff, y, textboxWidth, 20, MENU_NAME_TEXTBOX);
	//m_menuNameTextbox.SetText("NONE");

	int radioButtonWidth = 100;
	int radioWidth = radioButtonWidth * 3;
	int radioHeight = 25;

	y += 50;
	m_hudSizeText.Init(m_Window->GetHwnd(), "HUD Size", x, y, 100, 16, HUD_SIZE_TEXT);
	m_hudSizeContainer.Init(m_Window->GetHwnd(), x, y + 17, radioWidth, radioHeight, HUD_SIZE_RADIO_CONTAINER);
	m_hudSizeContainer.AddRadioButton(m_Window->GetHwnd(), true, "800x600", x, y + 17, radioButtonWidth, radioHeight, HUD_SIZE_RADIO_800X600);
	m_hudSizeContainer.AddRadioButton(m_Window->GetHwnd(), false, "1024x768", x + 100, y + 17, radioButtonWidth, radioHeight, HUD_SIZE_RADIO_1024X768);
	m_hudSizeContainer.AddRadioButton(m_Window->GetHwnd(), false, "1366x768", x + 200, y + 17, radioButtonWidth, radioHeight, HUD_SIZE_RADIO_1366X768);
	m_hudSizeContainer.AddRadioButton(m_Window->GetHwnd(), false, "1280x1024", x, y + 17 + radioHeight, radioButtonWidth, radioHeight, HUD_SIZE_RADIO_1280X1024);
	m_hudSizeContainer.AddRadioButton(m_Window->GetHwnd(), false, "1920x1080", x + 100, y + 17 + radioHeight, radioButtonWidth * 2, radioHeight, HUD_SIZE_RADIO_1920X1080);
	SendMessage(m_hudSizeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	/*
	radioButtonWidth = 100;
	radioWidth = radioButtonWidth * 2;

	y += 80;
	m_menuTypeText.Create(m_Window->GetHwnd(), "Menu Type", x, y, textWidth, 16, MENU_TYPE_TEXT);
	m_menuTypeContainer.Init(m_Window->GetHwnd(), x, y + 17, radioWidth, radioHeight, MENU_TYPE_RADIO_CONTAINER);
	m_menuTypeContainer.AddRadioButton(m_Window->GetHwnd(), true, "Main Menu", x, y + 17, radioButtonWidth, radioHeight, MENU_TYPE_RADIO_MAINMENU);
	m_menuTypeContainer.AddRadioButton(m_Window->GetHwnd(), false, "In Game Menu", x + 100, y + 17, radioButtonWidth + 30, radioHeight, MENU_TYPE_RADIO_INGAMEMENU);
	SendMessage(m_menuTypeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
	*/
	m_createButton.Init(m_Window->GetHwnd(), "Create", 295, 160, 60, 30, HUD_CREATE_BUTTON);
	m_closeButton.Init(m_Window->GetHwnd(), "Close", 360, 160, 60, 30, HUD_CLOSE_BUTTON);
#pragma endregion
}
//================================================================================================================
NewHUDWindow::NewHUDWindow(const NewHUDWindow& other)
{
}
//================================================================================================================
NewHUDWindow::~NewHUDWindow()
{
}
//================================================================================================================
void NewHUDWindow::SetPosition(int x, int y)
{
	m_Window->SetWindowPos(x, y);
}
//================================================================================================================
void NewHUDWindow::Invalidate()
{
	m_Window->Invalidate();
}
//================================================================================================================
/*void NewMenuWindow::CreateGUIComponents(HWND hwnd)
{
	//Get the background image path
	/*string path = CGlobal::FindInitialPath();
	path.append("\\Textures\\ImageCache.bmp");

	m_backgroundImage = (HBITMAP)LoadImage(NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (m_backgroundImage == NULL)
	{
		// LoadImage faled so get extended error information.
		DWORD dwError = ::GetLastError();

		MessageBox(m_hwnd, "LoadImage Failed", "CreateMenuWindow Error", MB_OK);
	}
	*
#pragma region "GUI"
	int x = 10;
	int y = 30;

	int textWidth = 100;
	int textboxXDiff = textWidth + 10;
	int textboxWidth = 100;

	m_menuNameText.Create(hwnd, "Menu Name", x, y, textWidth, 16, MENU_NAME_TEXT);
	m_menuNameTextbox.Init(hwnd, x + textboxXDiff, y, textboxWidth, 20, MENU_NAME_TEXTBOX);
	m_menuNameTextbox.SetText("NONE");

	int radioButtonWidth = 100;
	int radioWidth = radioButtonWidth * 3;
	int radioHeight = 25;

	y += 50;
	m_menuSizeText.Create(hwnd, "Menu Size", x, y, textWidth, 16, MENU_SIZE_TEXT);
	m_menuSizeContainer.Init(hwnd, x, y + 17, radioWidth, radioHeight, MENU_SIZE_RADIO_CONTAINER);
	m_menuSizeContainer.AddRadioButton(hwnd, true, "800x600", x, y + 17, radioButtonWidth, radioHeight, MENU_SIZE_RADIO_800X600);
	m_menuSizeContainer.AddRadioButton(hwnd, false, "1024x768", x + 100, y + 17, radioButtonWidth, radioHeight, MENU_SIZE_RADIO_1024X768);
	m_menuSizeContainer.AddRadioButton(hwnd, false, "1366x768", x + 200, y + 17, radioButtonWidth, radioHeight, MENU_SIZE_RADIO_1366X768);
	m_menuSizeContainer.AddRadioButton(hwnd, false, "1280x1024", x, y + 17 + radioHeight, radioButtonWidth, radioHeight, MENU_SIZE_RADIO_1280X1024);
	m_menuSizeContainer.AddRadioButton(hwnd, false, "1920x1080", x + 100, y + 17 + radioHeight, radioButtonWidth * 2, radioHeight, MENU_SIZE_RADIO_1920X1080);
	SendMessage(m_menuSizeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	radioButtonWidth = 100;
	radioWidth = radioButtonWidth * 2;

	y += 80;
	m_menuTypeText.Create(hwnd, "Menu Type", x, y, textWidth, 16, MENU_TYPE_TEXT);
	//m_menuTypeText->SetFont(16);
	//m_menuTypeText->SetText("Menu Type");
	m_menuTypeContainer.Init(hwnd, x, y + 17, radioWidth, radioHeight, MENU_TYPE_RADIO_CONTAINER);
	m_menuTypeContainer.AddRadioButton(hwnd, true, "Main Menu", x, y + 17, radioButtonWidth, radioHeight, MENU_TYPE_RADIO_MAINMENU);
	m_menuTypeContainer.AddRadioButton(hwnd, false, "In Game Menu", x + 100, y + 17, radioButtonWidth + 30, radioHeight, MENU_TYPE_RADIO_INGAMEMENU);
	SendMessage(m_menuTypeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	m_createButton.Init(hwnd, "Create", 295, 160, 60, 30, CREATE_BUTTON);
	m_closeButton.Init(hwnd, "Close", 360, 160, 60, 30, CLOSE_BUTTON);
#pragma endregion
}
*/
//================================================================================================================
/*void NewMenuWindow::BltBackground(HDC hdc)
{
	HDC hdcLocal = CreateCompatibleDC(hdc);

	if (hdcLocal == NULL)
	{
		MessageBox(m_hwnd, "CreateCompatibleDC Failed", "NewMenuWindow Error", MB_OK);
		return;
	}

	BITMAP g_background;

	int r = GetObject(reinterpret_cast<HGDIOBJ>(m_backgroundImage), sizeof(BITMAP), reinterpret_cast<LPVOID>(&g_background));

	if (!r)
	{
		MessageBox(m_hwnd, "GetObject Failed", "NewMenuWindow Error", MB_OK);
		return;
	}

	HBITMAP g_oldBackground = (HBITMAP)SelectObject(hdcLocal, m_backgroundImage);

	if (g_oldBackground == NULL)
	{
		MessageBox(m_hwnd, "SelectObject Failed", "NewMenuWindow Error", MB_OK);
		return;
	}

	BOOL result = BitBlt(hdc, 0, 0, g_background.bmWidth, g_background.bmHeight, hdcLocal, 0, 0, SRCCOPY);

	if (!result)
	{
		MessageBox(m_hwnd, "BitBlt Failed", "NewMenuWindow Error", MB_OK);
		return;
	}

	SelectObject(hdcLocal, g_oldBackground);
	DeleteDC(hdcLocal);
}
*/
//================================================================================================================
void NewHUDWindow::SetGD(GameDirectory2D* gd)
{
	this->gd = gd;
}
//================================================================================================================
void NewHUDWindow::AddHUDEditorSystem(HUDEditorSystem* hudEditorSystem)
{
	m_HUDEditorSystem = hudEditorSystem;
}
//================================================================================================================
/*void NewMenuWindow::AddMenuName(string name)
{
	m_createdMenus.push_back(name);
}*/
//================================================================================================================
LRESULT NewHUDWindow::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case HUD_CLOSE_BUTTON:
		{
			m_escape = true;
		}
		break;
		case HUD_CREATE_BUTTON:
		{
			/*string menu_name = m_menuNameTextbox.GetText();
			if (BetterString(menu_name) == "NONE" || BetterString(menu_name) == "")
			{
				ZShadeMessageCenter::MsgBoxError(m_Window->GetHwnd(), "Must have a name for the menu!!!");
			}
			else
			{
				m_created = true;
				m_escape = true;
				m_menuName = m_menuNameTextbox.GetText();

				// Create a new menu in the menu editor system
				m_MenuEditorSystem->CreateMenu(m_menuName);
			}*/
		}
		break;
		case HUD_SIZE_RADIO_800X600:
		{
			m_hudWidth = 800;
			m_hudHeight = 600;
		}
		break;
		case HUD_SIZE_RADIO_1024X768:
		{
			m_hudWidth = 1024;
			m_hudHeight = 768;
		}
		break;
		case HUD_SIZE_RADIO_1366X768:
		{
			m_hudWidth = 1366;
			m_hudHeight = 768;
		}
		break;
		case HUD_SIZE_RADIO_1280X1024:
		{
			m_hudWidth = 1280;
			m_hudHeight = 1024;
		}
		break;
		case HUD_SIZE_RADIO_1920X1080:
		{
			m_hudWidth = 1920;
			m_hudHeight = 1080;
		}
		break;
	}
	
	return 1;
}
//================================================================================================================
LRESULT NewHUDWindow::Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_D3DSystem->GDIPlus()->DrawRectangle(m_Window->GetHwnd(), RGBColor::Color::SystemColor(), 0, 0, m_windowWidth, m_windowHeight);
	
	return 1;
}
//================================================================================================================
bool NewHUDWindow::InFocus()
{
	if (GetForegroundWindow() != m_Window->GetHwnd())
	{
		return false;
	}

	return true;
}
//================================================================================================================
/*bool NewMenuWindow::OnCreate()
{
	m_createMenuWindowInstance = this;

	WNDCLASSEX winclass_ex;

	ZeroMemory(&winclass_ex, sizeof(WNDCLASSEX));

	winclass_ex.cbSize = sizeof(WNDCLASSEX);
	winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	winclass_ex.lpfnWndProc = (WNDPROC)CreateMenuWindowWndProc;
	winclass_ex.cbClsExtra = 0;
	winclass_ex.cbWndExtra = 0;
	winclass_ex.hInstance = GetModuleHandle(NULL);
	winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	winclass_ex.lpszMenuName = NULL;
	winclass_ex.lpszClassName = "CreateMenuWindowClass";
	winclass_ex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&winclass_ex))
	{
		MessageBox(NULL, "Registration Failed", "Error", MB_OK);
		return false;
	}

	int size_x = m_windowWidth;
	int size_y = m_windowHeight;

	m_hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,// | WS_EX_TOPMOST,
		"CreateMenuWindowClass",
		"Create Menu Window",
		WS_OVERLAPPED,
		CW_USEDEFAULT, CW_USEDEFAULT,
		size_x, size_y,
		NULL, NULL,
		GetModuleHandle(NULL),
		NULL);

	if (m_hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed", "Error", MB_OK);
		return false;
	}
}*/
//--------------------------------------------------------------------------------------------------------------------
int NewHUDWindow::Run()
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
//--------------------------------------------------------------------------------------------------------------------
void NewHUDWindow::Show(bool show)
{
	m_Window->Show(show);

	if (show)
	{
		m_running = true;
		m_escape = false;
		//SetFocus(m_hwnd);
		
		m_Window->Invalidate();
	}
	else
	{
		m_running = false;
		m_escape = true;
	}
}
//--------------------------------------------------------------------------------------------------------------------
/*LRESULT CALLBACK NewMenuWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			ProcessUpdates(wParam, lParam);
		}
		break;
		case WM_PAINT:
		{
			//CGlobal::ClientResize(hwnd, m_windowWidth, m_windowHeight);

			/*
			PAINTSTRUCT ps;
			m_winDC = BeginPaint(hwnd, &ps);
			BltBackground(m_winDC);
			EndPaint(hwnd, &ps);
			*
		}
		break;
		/*case WM_CTLCOLORSTATIC:
		{
			int iControlId = GetWindowLong((HWND)lParam, GWL_ID);
			switch (iControlId)
			{
			case MENU_SIZE_TEXT:
			case MENU_TYPE_TEXT:
			case MENU_NAME_TEXT:
			{
				SetTextColor((HDC)wParam, RGB(255, 255, 255));
				SetBkColor((HDC)wParam, RGB(255, 0, 0));
				SetBkMode((HDC)wParam, TRANSPARENT);
				return (LRESULT)((HBRUSH)GetStockObject(NULL_BRUSH));
			}
			break;
			}
		}
		break;*
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK CreateMenuWindowWndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	return m_createMenuWindowInstance->WinProc(hwnd, msg, wParam, lParam);
}*/
//--------------------------------------------------------------------------------------------------------------------
