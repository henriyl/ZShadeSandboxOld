//================================================================================================================
// Engine Includes
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
#include "Environment.h"
#include "FrontEnd.h"
#include "CGlobal.h"
#include "MemoryMgr.h"
#include "EnvironmentFactory.h"
//================================================================================================================
// Base Includes
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//Library Includes
#pragma comment(lib, "ComCtl32.Lib")
//================================================================================================================
ofstream pfile;
//================================================================================================================
//================================================================================================================
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	ZShadeINIParser iniParser("ZShadeSandbox.ini", true);
	
	string dx_version = iniParser.GetString("Editor", "DirectX");
	bool fullscreen = iniParser.GetBoolean("Editor", "Fullscreen");
	bool vsync = iniParser.GetBoolean("Editor", "vsync");
	bool fit_screen = iniParser.GetBoolean("Editor", "FitScreen");
	int screenWidth = iniParser.GetInt("Editor", "Screen Width");
	int screenHeight = iniParser.GetInt("Editor", "Screen Height");
	string dimension = iniParser.GetString("Editor", "Dim");
	string env3DType = iniParser.GetString("Editor", "Env3D");
	int renderDistance2D = iniParser.GetInt("Editor", "RenderDistance2D");
	string game_folder = iniParser.GetString("Editor", "GameFolder");
	bool remember = iniParser.GetBoolean("Editor", "RememberMode");
	string mode = iniParser.GetString("Editor", "Mode");
	float nearPlane = iniParser.GetFloat("Editor", "NearPlane");
	float farPlane = iniParser.GetFloat("Editor", "FarPlane");
	string type2d = iniParser.GetString("Editor", "Type2D");
	
	if (fit_screen)
	{
		//Find the maximum resolution support by the monitor
		CGlobal::GetDesktopResolution(screenWidth, screenHeight);
	}
	
	DimType dimType;
	if (dimension == "2D")
		dimType = ZSHADE_2D;
	else if (dimension == "3D")
		dimType = ZSHADE_3D;
	
	unique_ptr<EngineOptions> eo;
	eo.reset(new EngineOptions());

	eo->InitializeDefaults();
	
	if (dx_version == "11")
		eo->m_d3dVersion = DIRECTX11;
	else if (dx_version == "10")
		eo->m_d3dVersion = DIRECTX10;
	
	if (type2d == "Platformer")
		eo->m_GameType2D = PLATFORMER;
	else if (type2d == "Topdown")
		eo->m_GameType2D = TOPDOWN;
	
	eo->m_vsync = vsync;
	eo->fullscreen = fullscreen;
	eo->m_screenWidth = screenWidth;// / 2 + 200;
	eo->m_screenHeight = screenHeight;// -88;
	eo->m_DimType = dimType;
	eo->m_renderDistance2D = renderDistance2D;
	eo->hinstance = hInstance;
	eo->m_Env3DType = env3DType;
	eo->m_GameFolderName = game_folder;
	eo->fFarPlane = farPlane;
	eo->fNearPlane = nearPlane;
	
	if (!remember)
	{
		FrontEnd fe(eo.get());
		fe.Run();
	}
	else if (remember)
	{
		// Need a yes/no message box
		const int result = ZShadeMessageCenter::MsgBoxYesNo(NULL, "Would you like to run the remembered mode?", "RememberMode");

		switch (result)
		{
			case IDYES:
			{
				string name = "";
				bool init_lua = false;
				bool created = false;
				
				if (mode == "Editor")
				{
					eo->m_inEditor = true;
					name = "editor";
					init_lua = false;
					created = true;
				}
				else if (mode == "Game")
				{
					eo->m_inEditor = false;
					name = "game";
					init_lua = true;
					created = true;
				}
				
				if (created)
				{
					EnvironmentFactory::Instance()->CreateEnvironment(name, eo.get(), "BaseWindow", "RenderWindow", init_lua);
					EnvironmentFactory::Instance()->ActiveEnvironmentName() = name;
					EnvironmentFactory::Instance()->SelectEnvironment(name)->Run();
				}
			}
			break;
			case IDNO:// Go back to FrontEnd loader
			{
				FrontEnd fe(eo.get());
				fe.Run();
			}
			break;
		}
	}
	
	return 0;
}
//================================================================================================================