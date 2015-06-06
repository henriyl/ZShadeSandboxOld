#include "FrontEnd.h"
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
//#include "EditorEnvironment.h"
#include "Environment.h"
#include <functional>
#include "EnvironmentFactory.h"
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//==================================================================================================================================
//==================================================================================================================================
FrontEnd::FrontEnd(EngineOptions* eo)
:   m_EngineOptions(eo)
,   m_remember(false)
{
	Init();
}
//================================================================================================================
int FrontEnd::Run()
{
	while (m_Window->IsAlive())
	{
		if (!m_Window->WinIsMinimized())
		{
			m_EngineOptions->m_GameFolderName = m_GameFolderText.GetText();

			//Quick esc for the game
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				//Exit();
				break;
			}
		}

		m_Window->Run();
	}

	return 1;
}
//==================================================================================================================================
bool FrontEnd::Init()
{
	m_Window = new Window();
	
	if (!m_Window->Init(m_EngineOptions->hinstance, false, 200, 350, "ZShadeSandbox", "ZShade FrontEnd Class", NULL, IDI_ICON, IDC_TARGET, false, false))
	{
		ZShadeMessageCenter::MsgBoxError(NULL, "FrontEnd Window Creation Failed");
		return false;
	}
	
	LONG_PTR style = GetWindowLongPtr(m_Window->GetHwnd(), GWL_STYLE);
	SetWindowLongPtr(m_Window->GetHwnd(), GWL_STYLE, style & ~WS_SYSMENU);

	// TODO: Need to center this window
	m_Window->SetWindowPos(m_EngineOptions->m_screenWidth / 2, m_EngineOptions->m_screenHeight / 2);
	m_Window->Show();
	m_Window->SetUserMessageFunction(WM_COMMAND, bind(mem_fn(&FrontEnd::Command), this, _1, _2, _3, _4));
	
	int x = 40;
	int y = 40;
	
	m_Label.Init(m_Window->GetHwnd(), "Select Mode", x, y, 90, 16, MODE_LABEL);
	
	//
	// Create the buttons for selecting editor mode or game mode
	//
	
	y += 30;
	m_EditorModeButton.Init(m_Window->GetHwnd(), "Editor Mode", x, y, 100, 30, EDITOR_MODE_BUTTON);
	
	y += 40;
	m_GameModeButton.Init(m_Window->GetHwnd(), "Game Mode", x, y, 100, 30, GAME_MODE_BUTTON);
	
	y += 40;
	m_ExitButton.Init(m_Window->GetHwnd(), "Exit", x, y, 100, 30, EXIT_BUTTON);
	
	// TODO: Get the GameFolder name from a textbox for when the Game Mode button is pressed
	// Get the GameFolder name from the ini file, update the ini file with the GameFolder from the textbox
	// Add a Checkbox called remember and update it in the ini file
	// When remember is true a yes/no message box will pop up when the engine is ran and ask the user if they would like to load remembered mode.
	
	x = 10;
	y += 60;
	m_GameFolderLabel.Init(m_Window->GetHwnd(), "Game Folder", x, y, 90, 16, GAME_FOLDER_LABEL);
	y += 20;
	m_GameFolderText.Init(m_Window->GetHwnd(), x, y, 100, 20, GAME_FOLDER_TEXT);
	
	y += 25;
	m_RememberCheckbox.Init(m_Window->GetHwnd(), "Remember", x, y, 90, 16, REMEMBER_CHECKBOX);

	// Read the GameFolder and Remember values from the ini file
	ZShadeINIParser iniParser("ZShadeSandbox.ini", true);

	string game_folder = iniParser.GetString("Editor", "GameFolder");
	string remember = iniParser.GetString("Editor", "RememberMode");

	m_GameFolderText.SetText(game_folder);

	if (BetterString(remember) == "false")
		m_RememberCheckbox.Check(false);
	else if (BetterString(remember) == "true")
		m_RememberCheckbox.Check(true);

	return true;
}
//==================================================================================================================================
bool FrontEnd::InitEditorMode()
{
	m_Window->Show(false);
	m_Window->Destroy();
	
	m_EngineOptions->m_inEditor = true;

	EnvironmentFactory::Instance()->CreateEnvironment("editor", m_EngineOptions, "BaseWindow", "RenderWindow", false);
	EnvironmentFactory::Instance()->ActiveEnvironmentName() = "editor";
	EnvironmentFactory::Instance()->SelectEnvironment("editor")->Run();
	
	return true;
}
//==================================================================================================================================
bool FrontEnd::InitGameMode()
{
	m_Window->Show(false);
	m_Window->Destroy();
	
	m_EngineOptions->m_inEditor = false;

	if (m_EngineOptions->m_GameFolderName != "NONE" || m_EngineOptions->m_GameFolderName != "")
	{
		ZShadeINIParser iniParser("ZShadeSandbox.ini", true);
		iniParser.WriteString("Editor", "GameFolder", m_EngineOptions->m_GameFolderName);

		EnvironmentFactory::Instance()->CreateEnvironment("game", m_EngineOptions, "BaseWindow", "RenderWindow", true);
		EnvironmentFactory::Instance()->ActiveEnvironmentName() = "game";
		EnvironmentFactory::Instance()->SelectEnvironment("game")->Run();
	}

	return true;
}
//==================================================================================================================================
LRESULT FrontEnd::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case EDITOR_MODE_BUTTON:
		{
			// Create and Run EditorEnvironment
			// Make this window disappear when this button is pressed
			InitEditorMode();
		}
		break;
		case GAME_MODE_BUTTON:
		{
			// Create and Run Environment for game
			// Make this window disappear when this button is pressed
			InitGameMode();
		}
		break;
		case EXIT_BUTTON:
		{
			// Exit the application
			ExitProcess(0);
		}
		break;
		case REMEMBER_CHECKBOX:
		{
			m_remember = !m_remember;

			ZShadeINIParser iniParser("ZShadeSandbox.ini", true);

			if (m_remember)
				iniParser.WriteString("Editor", "RememberMode", "true");
			else
				iniParser.WriteString("Editor", "RememberMode", "false");
		}
		break;
	}
}
//==================================================================================================================================