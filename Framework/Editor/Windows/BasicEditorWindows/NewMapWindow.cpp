#include <windowsx.h>
#include "NewMapWindow.h"
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
NewMapWindow::NewMapWindow(MapEditorSystem2D* mapEditorSystem, HINSTANCE hInstance, D3D* d3d)
:   m_running(false)
,   m_xSize(0)
,   m_ySize(0)
,   m_fowEnabled(false)
,   m_windowWidth(450)
,   m_windowHeight(300)
,   m_fowRadius(-1)
,	m_created(false)
,   m_mapEditorSystem2D(mapEditorSystem)
,   bIso(false)
,   m_WorldName("")
,	m_mapCreated(false)
,   m_fowFlashlight(false)
,   m_D3DSystem(d3d)
{
	m_Window = new Window();
	
	if (!m_Window->Init(hInstance, false, m_windowWidth, m_windowHeight, "New Map Window", "NewMapWindow", NULL))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "NewMapWindow Creation Failed");
	}
	
	m_Window->SetUserMessageFunction(WM_COMMAND, bind(mem_fn(&NewMapWindow::Command), this, _1, _2, _3, _4));
	m_Window->SetUserMessageFunction(WM_PAINT, bind(mem_fn(&NewMapWindow::Paint), this, _1, _2, _3, _4));
	m_Window->SetUserMessageFunction(WM_CTLCOLORSTATIC, bind(mem_fn(&NewMapWindow::ColorStatic), this, _1, _2, _3, _4));
	
	int x = 30;
	int y = 30;
	
	m_mapSizeLabel.Init(m_Window->GetHwnd(), "Map Size", x, y + 5, 50, 12, MAP_SIZE_TEXT);
	m_mapSizeCombobox.Init(m_Window->GetHwnd(), x + 60, y, 80, 500, MAP_SIZE_CB);
	m_mapSizeCombobox.AddItem("1x1");
	m_mapSizeCombobox.AddItem("3x3");
	m_mapSizeCombobox.AddItem("6x6");
	m_mapSizeCombobox.AddItem("8x8");
	m_mapSizeCombobox.AddItem("10x10");
	m_mapSizeCombobox.AddItem("20x20");
	m_mapSizeCombobox.AddItem("30x30");
	m_mapSizeCombobox.SetDisplayItem("1x1", 0);
	
	y = 70;
	m_IsoStandLabel.Init(m_Window->GetHwnd(), "Map Type", x, y, 80, 12, ISO_STAND_TEXT);
	y += 15;
	m_IsoStandRadioContainer.Init(m_Window->GetHwnd(), x, y, 200, 50, ISO_STAND_RG);
	m_IsoStandRadioContainer.AddRadioButton(m_Window->GetHwnd(), true, "STANDARD", x, y, 100, 50, STANDARD_RADIO_BUTTON);
	m_IsoStandRadioContainer.AddRadioButton(m_Window->GetHwnd(), false, "ISOMETRIC", x + 100, y, 100, 50, ISOMETRIC_RADIO_BUTTON);
	SendMessage(m_IsoStandRadioContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	y += 60;
	m_worldNameLabel.Init(m_Window->GetHwnd(), "World Name", x, y + 5, 50, 12, WORLD_NAME_TEXT);
	m_worldNameComboBox.Init(m_Window->GetHwnd(), x + 60, y, 80, 500, WORLD_NAME_CB);

	y += 30;
	m_IsoStandLabel.Init(m_Window->GetHwnd(), "Map Name", x, y, 60, 12, MAP_NAME_TEXT);
	m_MapNameTextbox.Init(m_Window->GetHwnd(), x + 60, y, 100, 20, MAP_NAME);

	y = 30;
	x = 270;
	m_fowEnabledLabel.Init(m_Window->GetHwnd(), "Enable FOW", x, y, 80, 12, FOW_ENABLED_TEXT);
	y += 15;
	m_fowEnabledRadioContainer.Init(m_Window->GetHwnd(), x, y, 100, 50, FOW_ENABLED_RC);
	m_fowEnabledRadioContainer.AddRadioButton(m_Window->GetHwnd(), true, "Yes", x, y, 50, 50, YES_RADIO_BUTTON);
	m_fowEnabledRadioContainer.AddRadioButton(m_Window->GetHwnd(), false, "No", x + 50, y, 50, 50, NO_RADIO_BUTTON);
	SendMessage(m_fowEnabledRadioContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	y += 70;
	m_fowFlashlightEnabled.Init(m_Window->GetHwnd(), "FOW Flashlight", x, y, 130, 20, FOW_FLASHLIGHT);
	m_fowFlashlightEnabled.Check(false);
	EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);

	y += 20;
	m_fowRadiusLabel.Init(m_Window->GetHwnd(), "FOW Radius", x, y + 5, 80, 12, FOW_RADIUS_TEXT);
	m_fowRadiusComboBox.Init(m_Window->GetHwnd(), x, y + 20, 80, 500, FOW_RADIUS_CB);
	m_fowRadiusComboBox.AddItem("3");
	m_fowRadiusComboBox.AddItem("4");
	m_fowRadiusComboBox.AddItem("5");
	m_fowRadiusComboBox.AddItem("6");
	m_fowRadiusComboBox.AddItem("7");
	m_fowRadiusComboBox.AddItem("8");
	m_fowRadiusComboBox.AddItem("9");
	m_fowRadiusComboBox.SetDisplayItem("3", 0);

	EnableWindow(m_fowRadiusComboBox.GetHwnd(), m_fowEnabled);

	m_createButton.Init(m_Window->GetHwnd(), "Create", x, 190, 60, 30, CREATE_BUTTON);
	m_closeButton.Init(m_Window->GetHwnd(), "Close", x + 70, 190, 60, 30, CLOSE_BUTTON);
}
//================================================================================================================
NewMapWindow::NewMapWindow(const NewMapWindow& other)
{
}
//================================================================================================================
NewMapWindow::~NewMapWindow()
{
}
//================================================================================================================
void NewMapWindow::SetPosition(int x, int y)
{
	m_Window->SetWindowPos(x, y);
}
//================================================================================================================
void NewMapWindow::Invalidate()
{
	m_Window->Invalidate();
}
//================================================================================================================
void NewMapWindow::AddWorldName(string name)
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
/*void NewMapWindow::CreateGUIComponents(HWND hwnd)
{
	int x = 30;
	int y = 30;

	m_createButton = new WinButton(hwnd, "Create", x, 210, 60, 30, CREATE_BUTTON);
	m_closeButton = new WinButton(hwnd, "Close", x + 70, 210, 60, 30, CLOSE_BUTTON);

	m_mapSizeLabel = new WinText(hwnd, "Map Size", x, y + 5, 50, 12, MAP_SIZE_TEXT);
	m_mapSizeCombobox = new WinComboBox(hwnd, x + 60, y, 80, 500, MAP_SIZE_CB);
	m_mapSizeCombobox->AddItem("1x1");
	m_mapSizeCombobox->AddItem("3x3");
	m_mapSizeCombobox->AddItem("6x6");
	m_mapSizeCombobox->AddItem("8x8");
	m_mapSizeCombobox->AddItem("10x10");
	m_mapSizeCombobox->AddItem("20x20");
	m_mapSizeCombobox->AddItem("30x30");
	m_mapSizeCombobox->SetDisplayItem("1x1", 0);

	y = 70;
	m_IsoStandLabel = new WinText(hwnd, "Map Type", x, y, 80, 12, ISO_STAND_TEXT);
	y += 15;
	m_IsoStandRadioContainer = new WinRadioGroup(hwnd, x, y, 200, 50, ISO_STAND_RG);
	m_IsoStandRadioContainer->AddRadioButton(hwnd, true, "STANDARD", x, y, 100, 50, STANDARD_RADIO_BUTTON);
	m_IsoStandRadioContainer->AddRadioButton(hwnd, false, "ISOMETRIC", x + 100, y, 100, 50, ISOMETRIC_RADIO_BUTTON);
	SendMessage(m_IsoStandRadioContainer->GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	y += 60;
	m_worldNameLabel = new WinText(hwnd, "World Name", x, y + 5, 50, 12, WORLD_NAME_TEXT);
	m_worldNameComboBox = new WinComboBox(hwnd, x + 60, y, 80, 500, WORLD_NAME_CB);

	y += 30;
	m_IsoStandLabel = new WinText(hwnd, "Map Name", x, y, 60, 12, MAP_NAME_TEXT);
	m_MapNameTextbox = new WinTextbox(hwnd, x + 60, y, 100, 20, MAP_NAME);

	y = 30;
	x = 270;
	m_fowEnabledLabel = new WinText(hwnd, "Enable FOW", x, y, 80, 12, FOW_ENABLED_TEXT);
	y += 15;
	m_fowEnabledRadioContainer = new WinRadioGroup(hwnd, x, y, 100, 50, FOW_ENABLED_RC);
	m_fowEnabledRadioContainer->AddRadioButton(hwnd, true, "Yes", x, y, 50, 50, YES_RADIO_BUTTON);
	m_fowEnabledRadioContainer->AddRadioButton(hwnd, false, "No", x + 50, y, 50, 50, NO_RADIO_BUTTON);
	SendMessage(m_fowEnabledRadioContainer->GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	y += 70;
	m_fowFlashlightEnabled.Init(hwnd, "FOW Flashlight", x, y, 130, 20, FOW_FLASHLIGHT);
	m_fowFlashlightEnabled.Check(false);
	EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);

	y += 20;
	m_fowRadiusLabel = new WinText(hwnd, "FOW Radius", x, y + 5, 80, 12, FOW_RADIUS_TEXT);
	m_fowRadiusComboBox = new WinComboBox(hwnd, x, y + 20, 80, 500, FOW_RADIUS_CB);
	m_fowRadiusComboBox->AddItem("3");
	m_fowRadiusComboBox->AddItem("4");
	m_fowRadiusComboBox->AddItem("5");
	m_fowRadiusComboBox->AddItem("6");
	m_fowRadiusComboBox->AddItem("7");
	m_fowRadiusComboBox->AddItem("8");
	m_fowRadiusComboBox->AddItem("9");
	m_fowRadiusComboBox->SetDisplayItem("3", 0);

	EnableWindow(m_fowRadiusComboBox->GetMe(), m_fowEnabled);
}*/
//================================================================================================================
/*void NewMapWindow::BltBackground(HDC hdc)
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
//================================================================================================================
/*void NewMapWindow::ProcessUpdates(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	m_mapSizeCombobox->UpdateCommand(wParam, lParam);
	m_fowRadiusComboBox->UpdateCommand(wParam, lParam);
	m_worldNameComboBox->UpdateCommand(wParam, lParam);

	switch (LOWORD(wParam))
	{
		case CREATE_BUTTON:
		{
			//Get all the attributes

			//Get the size of the map
			string str = m_mapSizeCombobox->GetSelectedItem();
			if (str != "")
			{
				BetterString bstr(str);
				vector<string> mapSizeText_vec = bstr.split('x');
				if (mapSizeText_vec.size() > 0)
				{
					m_xSize = atoi(mapSizeText_vec[0].c_str());
					m_ySize = atoi(mapSizeText_vec[1].c_str());
				}
			}
			
			//Is FOW enabled
			if (m_fowEnabled)
			{
				//Get the radius of the FOW
				string radiusText = m_fowRadiusComboBox->GetSelectedItem();
				if (radiusText != "")
				{
					m_fowRadius = atoi(string(radiusText).c_str());
				}

				// Is it flashlight fow
				m_fowFlashlight = m_fowFlashlightEnabled.IsChecked();
			}

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

					m_mapEditorSystem2D->NewMapTopdown(m_WorldName, m_MapNameTextbox->GetText(),
						m_xSize, m_ySize, m_fowEnabled, m_fowRadius, m_fowFlashlight, bIso,
						true, m_hwndProgressBar);
				}
			}
		}
		break;
		case CLOSE_BUTTON:
		{
			m_escape = true;
		}
		break;
		case YES_RADIO_BUTTON:
		{
			m_fowEnabled = true;
			EnableWindow(m_fowRadiusComboBox->GetMe(), m_fowEnabled);
			EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);
		}
		break;
		case NO_RADIO_BUTTON:
		{
			m_fowEnabled = false;
			EnableWindow(m_fowRadiusComboBox->GetMe(), m_fowEnabled);
			EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);
		}
		break;
		case ISOMETRIC_RADIO_BUTTON:
		{
			bIso = true;
		}
		break;
		case STANDARD_RADIO_BUTTON:
		{
			bIso = false;
		}
		break;
	}
}
//================================================================================================================
bool NewMapWindow::OnCreate()
{
	m_newMapInstance = this;

	WNDCLASSEX winclass_ex;

	ZeroMemory(&winclass_ex, sizeof(WNDCLASSEX));

	winclass_ex.cbSize = sizeof(WNDCLASSEX);
	winclass_ex.style = CS_HREDRAW | CS_VREDRAW;
	winclass_ex.lpfnWndProc = (WNDPROC)NewTopdownMapWndProc;
	winclass_ex.cbClsExtra = 0;
	winclass_ex.cbWndExtra = 0;
	winclass_ex.hInstance = GetModuleHandle(NULL);
	winclass_ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass_ex.hbrBackground = (HBRUSH)GetStockObject( GRAY_BRUSH );//(HBRUSH)(COLOR_WINDOW + 1);
	winclass_ex.lpszMenuName = NULL;
	winclass_ex.lpszClassName = "NewMapClass";
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
		"NewMapClass",
		"New Map",
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
bool NewMapWindow::InFocus()
{
	if (GetForegroundWindow() == m_Window->GetHwnd())
		return true;
	else
		return false;
}
//================================================================================================================
int NewMapWindow::Run()
{
	/*
	MSG msg;
	bool done = false;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			if (m_escape)
				done = true;
		}

		////If Escape Key is pressed (bail out)
		//if (m_inputSystem->isEscapePressed())
		//{
		//	done = true;
		//}
	}

	m_newMapInstance->Show( false );
	return msg.wParam;
	*/
	
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
void NewMapWindow::Show(bool show)
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
LRESULT NewMapWindow::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_mapSizeCombobox.UpdateCommand(wParam, lParam);
	m_fowRadiusComboBox.UpdateCommand(wParam, lParam);
	m_worldNameComboBox.UpdateCommand(wParam, lParam);

	switch (LOWORD(wParam))
	{
		case CREATE_BUTTON:
		{
			//Get all the attributes

			//Get the size of the map
			string str = m_mapSizeCombobox.GetSelectedItem();
			if (str != "")
			{
				BetterString bstr(str);
				vector<string> mapSizeText_vec = bstr.split('x');
				if (mapSizeText_vec.size() > 0)
				{
					m_xSize = atoi(mapSizeText_vec[0].c_str());
					m_ySize = atoi(mapSizeText_vec[1].c_str());
				}
			}
			
			//Is FOW enabled
			if (m_fowEnabled)
			{
				//Get the radius of the FOW
				string radiusText = m_fowRadiusComboBox.GetSelectedItem();
				if (radiusText != "")
				{
					m_fowRadius = atoi(string(radiusText).c_str());
				}

				// Is it flashlight fow
				m_fowFlashlight = m_fowFlashlightEnabled.IsChecked();
			}

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

					m_mapEditorSystem2D->NewMapTopdown(m_WorldName, m_MapNameTextbox.GetText(),
						m_xSize, m_ySize, m_fowEnabled, m_fowRadius, m_fowFlashlight, bIso,
						true, m_hwndProgressBar);
				}
			}
		}
		break;
		case CLOSE_BUTTON:
		{
			m_escape = true;
		}
		break;
		case YES_RADIO_BUTTON:
		{
			m_fowEnabled = true;
			EnableWindow(m_fowRadiusComboBox.GetHwnd(), m_fowEnabled);
			EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);
		}
		break;
		case NO_RADIO_BUTTON:
		{
			m_fowEnabled = false;
			EnableWindow(m_fowRadiusComboBox.GetHwnd(), m_fowEnabled);
			EnableWindow(m_fowFlashlightEnabled.GetHwnd(), m_fowEnabled);
		}
		break;
		case ISOMETRIC_RADIO_BUTTON:
		{
			bIso = true;
		}
		break;
		case STANDARD_RADIO_BUTTON:
		{
			bIso = false;
		}
		break;
	}
	
	return 1;
}
//================================================================================================================
LRESULT NewMapWindow::Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_D3DSystem->GDIPlus()->DrawRectangle(hwnd, RGBColor::Color::SystemColor(), 0, 0, m_windowWidth, m_windowHeight);
	
	return 1;
}
//================================================================================================================
LRESULT NewMapWindow::ColorStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int iControlId = GetWindowLong((HWND)lParam, GWL_ID);
	switch (iControlId)
	{
		case MAP_SIZE_TEXT:
		case FOW_ENABLED_TEXT:
		case FOW_RADIUS_TEXT:
		case ISO_STAND_TEXT:
		case MAP_NAME_TEXT:
		case WORLD_NAME_TEXT:
		{
			SetTextColor((HDC)wParam, RGB(0, 0, 0));
			SetBkColor((HDC)wParam, RGB(255, 0, 0));
			SetBkMode((HDC)wParam, TRANSPARENT);
			return (LRESULT)((HBRUSH)GetStockObject(NULL_BRUSH));
		}
		break;
	}

	return true;
}
/*
LRESULT CALLBACK NewMapWindow::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			//CGlobal::ClientResize(hwnd, m_windowWidth, m_windowHeight);
			/*PAINTSTRUCT ps;
			m_winDC = BeginPaint(hwnd, &ps);
			BltBackground(m_winDC);
			EndPaint(hwnd, &ps);*
			
			m_D3DSystem->GDIPlus()->DrawRectangle();
		}
		break;
		case WM_CTLCOLORSTATIC:
		{
			int iControlId = GetWindowLong((HWND) lParam, GWL_ID);
			switch(iControlId)
			{
				case MAP_SIZE_TEXT:
				case FOW_ENABLED_TEXT:
				case FOW_RADIUS_TEXT:
				case ISO_STAND_TEXT:
				case MAP_NAME_TEXT:
				case WORLD_NAME_TEXT:
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
*/
//================================================================================================================
//LRESULT CALLBACK NewTopdownMapWndProc(
//	HWND hwnd,
//	UINT msg,
//	WPARAM wParam,
//	LPARAM lParam)
//{
//	return m_newMapInstance->WinProc(hwnd, msg, wParam, lParam);
//}
//--------------------------------------------------------------------------------------------------------------------