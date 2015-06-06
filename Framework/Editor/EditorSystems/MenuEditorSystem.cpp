#include "MenuEditorSystem.h"
#include "Convert.h"
#include "EnvironmentMenuHelper.h"
#include "Button.h"
//================================================================================================================
MenuEditorSystem::MenuEditorSystem(Environment2D* env2D)
: m_Env2D(env2D)
{
	m_mainGameDirectory = m_Env2D->GetGD2D();
	m_D3DSystem = m_Env2D->GetD3DSystem();
	m_EngineOptions = m_Env2D->GetEngineOptions();

	m_ClickDelayTimer = new SleepTimer();
	m_SelectionTimer = new SleepTimer();
	
	m_SelectedButtonImageName = "";

	m_displaySpriteCover = 0;
	
	// Set defaults
	SwitchToEditTypeNone();

	usingGrid = false;
	gridSizeX = 32;
	gridSizeY = 32;

	m_leftButtonDown = false;

	mousePosText.SetD3D(m_D3DSystem);
	mousePosText.Initialize();
	mousePosText.CreateShadow();
	mousePosText.ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
	mousePosText.TextName() = "I am a Sprite";
	mousePosText.TextColor() = XMFLOAT4(1, 0, 0, 1);
}
//================================================================================================================
MenuEditorSystem::~MenuEditorSystem()
{
}
//================================================================================================================
void MenuEditorSystem::GoFullscreen(bool bEnable)
{
	m_Env2D->GoFullscreen(bEnable);
}
//================================================================================================================
void MenuEditorSystem::LoadRequiredSprites()
{
	/*string base_path = m_mainGameDirectory->m_required_textures;

	m_StampNormal = new Sprite(m_D3DSystem);
	m_StampNormal->Initialize(base_path, "stamp_normal.png", XMFLOAT3(0, 0, 0), 64, 64, STATIC);
	m_StampHighlight = new Sprite(m_D3DSystem);
	m_StampHighlight->Initialize(base_path, "stamp_highlight.png", XMFLOAT3(0, 0, 0), 64, 64, STATIC);

	m_StampNormal32x32 = new Sprite(m_D3DSystem);
	m_StampNormal32x32->Initialize(base_path, "stamp_normal_32x32.png", XMFLOAT3(0, 0, 0), 32, 32, STATIC);
	m_StampHighlight32x32 = new Sprite(m_D3DSystem);
	m_StampHighlight32x32->Initialize(base_path, "stamp_highlight_32x32.png", XMFLOAT3(0, 0, 0), 32, 32, STATIC);

	int tx = 64;
	int ty = 64;
	m_TileEdit1x1Sprite = new Sprite(m_D3DSystem);
	m_TileEdit1x1Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_1x1.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit2x2Sprite = new Sprite(m_D3DSystem);
	m_TileEdit2x2Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_2x2.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit3x3Sprite = new Sprite(m_D3DSystem);
	m_TileEdit3x3Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_3x3.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit4x4Sprite = new Sprite(m_D3DSystem);
	m_TileEdit4x4Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_4x4.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit5x5Sprite = new Sprite(m_D3DSystem);
	m_TileEdit5x5Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_5x5.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit6x6Sprite = new Sprite(m_D3DSystem);
	m_TileEdit6x6Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_6x6.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit7x7Sprite = new Sprite(m_D3DSystem);
	m_TileEdit7x7Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_7x7.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit8x8Sprite = new Sprite(m_D3DSystem);
	m_TileEdit8x8Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_8x8.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit9x9Sprite = new Sprite(m_D3DSystem);
	m_TileEdit9x9Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_9x9.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit10x10Sprite = new Sprite(m_D3DSystem);
	m_TileEdit10x10Sprite->Initialize(base_path, "MultipleStampTool\\stamp_normal_10x10.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);

	tx = 64;
	ty = 64;
	m_TileEdit1x1Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit1x1Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_1x1.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit2x2Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit2x2Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_2x2.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit3x3Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit3x3Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_3x3.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit4x4Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit4x4Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_4x4.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit5x5Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit5x5Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_5x5.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit6x6Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit6x6Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_6x6.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit7x7Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit7x7Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_7x7.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit8x8Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit8x8Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_8x8.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit9x9Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit9x9Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_9x9.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);
	tx += 64;
	ty += 64;
	m_TileEdit10x10Sprite2 = new Sprite(m_D3DSystem);
	m_TileEdit10x10Sprite2->Initialize(base_path, "MultipleStampTool\\stamp_highlight_10x10.png", XMFLOAT3(0, 0, 0), tx, ty, STATIC);

	int size = 150;
	if (m_EngineOptions->m_screenWidth == 1920 && m_EngineOptions->m_screenHeight == 1080)
		size += 50;

	m_displaySpriteCover = new Sprite(m_D3DSystem);
	m_displaySpriteCover->Initialize(base_path, "sprite_display_cover.png",
		XMFLOAT3(m_EngineOptions->m_screenWidth - size, m_EngineOptions->m_screenHeight - size, 0), 74, 74, STATIC);

	m_sectionSprite = new Sprite(m_D3DSystem);
	m_sectionSprite->Initialize(base_path, "highlight.png", XMFLOAT3(0, 0, 0),
		m_EngineOptions->ROW_SIZE * m_EngineOptions->TILE_SIZE, m_EngineOptions->COL_SIZE * m_EngineOptions->TILE_SIZE, STATIC);

	// Load FOW textures
	FogData::Instance()->LoadTextures(base_path);*/
}
//================================================================================================================
void MenuEditorSystem::UpdateTitleBar()
{
	if (m_Updated)
	{
		//Place an [*] on the title bar of the window
		string path = "";
		
		if (m_mainGameDirectory->GAME_FOLDER == "")
		{
			path = "ZShadeSandbox [*]";
		}
		else
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(") [*]");
		}
		//SetWindowText(m_Env2D->GetBaseWindow()->GetHwnd(), path.c_str());
	}
	else
	{
		string path = "";
		if (m_mainGameDirectory->GAME_FOLDER == "")
		{
			path = "ZShadeSandbox";
		}
		else
		{
			path = "ZShadeSandbox (";
			path.append(m_mainGameDirectory->GAME_FOLDER);
			path.append(")");
		}
		//SetWindowText(m_Env2D->GetBaseWindow()->GetHwnd(), path.c_str());
	}
}
//================================================================================================================
void MenuEditorSystem::SetGameDirectory(GameDirectory2D* gd)
{
	m_mainGameDirectory = gd;
	m_Env2D->SetGameDirectory(gd);
}
//================================================================================================================
void MenuEditorSystem::UpdateInput(Keyboard* keyboard)
{
	/*m_ClickDelayTimer->Sleep(10);
	if (m_ClickDelayTimer->IsTimeUp())
	{
		if (keyboard->IsKeyDown(Keyboard::Key::Left) || keyboard->IsKeyDown(Keyboard::Key::A))
		{
			if (ScrollOffset.x > 0)
				ScrollOffset.x -= edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Right) || keyboard->IsKeyDown(Keyboard::Key::D))
		{
			//if (ScrollOffset.x < 16)
				ScrollOffset.x += edgeScrollX;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Up) || keyboard->IsKeyDown(Keyboard::Key::W))
		{
			if (ScrollOffset.y > 0)
				ScrollOffset.y -= edgeScrollY;
		}

		if (keyboard->IsKeyDown(Keyboard::Key::Down) || keyboard->IsKeyDown(Keyboard::Key::S))
		{
			//if (ScrollOffset.y < 16)
				ScrollOffset.y += edgeScrollY;
		}
	}*/
}
//================================================================================================================
void MenuEditorSystem::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_leftButtonDown = true;
		
		m_SelectedMousePos.x = x;
		m_SelectedMousePos.y = y;
		
		// If applicable, Add a button
		if (BetterString(m_SelectedButtonImageName) != "")
		{
			AddButton();
		}
		
		// If applicable, Add a text
		AddText();
	}
}
//================================================================================================================
void MenuEditorSystem::OnMouseUp(WPARAM btnState, int x, int y)
{
	// Stop dragging of item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_leftButtonDown = false;
	}
}
//================================================================================================================
void MenuEditorSystem::OnMouseMove(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_SelectedMousePos.x = x;
		m_SelectedMousePos.y = y;
		
		m_leftButtonDown = true;
		
		// If applicable, Add a button
		if (BetterString(m_SelectedButtonImageName) != "")
		{
			AddButton();
		}
		
		// If applicable, Add a text
		AddText();
	}
	else
	{
		m_leftButtonDown = false;
	}
	
	m_MouseMovePos = SnapToGrid(x, y);

	// Move a button or text display cover
	if (editType == ET_Button && (action == A_Place || action == A_Move))
	{
		if (m_displaySpriteCover == NULL)
		{
			if (BetterString(m_SelectedButtonImageName) != "")
			{
				m_displaySpriteCover = new Sprite(m_D3DSystem);
				m_displaySpriteCover->Initialize(m_mainGameDirectory->m_menu_sprites_path, m_SelectedButtonImageName, XMFLOAT3(m_MouseMovePos.x, m_MouseMovePos.y, 0), 80, 30, PhysicsType::STATIC);
			}
		}
	}
}
//================================================================================================================
XMFLOAT2 MenuEditorSystem::SnapToGrid(int x, int y)
{
	XMFLOAT2 point = XMFLOAT2(x, y);
	
	if (usingGrid)
	{
		point.x = (x / gridSizeX) * gridSizeX;
		point.y = (y / gridSizeY) * gridSizeY;
	}
	
	return point;
}
//================================================================================================================
void MenuEditorSystem::Render(Camera* camera)
{
	// Can render display cover sprites here for buttons and text

	if (m_displaySpriteCover != NULL)
	{
		m_displaySpriteCover->TopLeftPosition() = XMFLOAT3(m_MouseMovePos.x, m_MouseMovePos.y, 0);
		m_D3DSystem->TurnOnAlphaBlending();
			m_displaySpriteCover->Render(camera);
		m_D3DSystem->TurnOffAlphaBlending();
	}
}
//================================================================================================================
void MenuEditorSystem::GrabMenuNames(vector<string>& names)
{
	EnvironmentMenuHelper::GrabMenuNames(names);
}
//================================================================================================================
void MenuEditorSystem::SetActiveRenderedMenu(string menu_name)
{
	EnvironmentMenuHelper::SetActiveRenderedMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::RemoveMenu(string menu_name)
{
	EnvironmentMenuHelper::RemoveMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::CreateMenu(string menu_name)
{
	EnvironmentMenuHelper::CreateMenu(menu_name);
}
//================================================================================================================
void MenuEditorSystem::LoadAMenu(string menu_filename, string& menuName)
{
	EnvironmentMenuHelper::LoadAMenu(menu_filename, menuName);
}
//================================================================================================================
void MenuEditorSystem::SaveMenu(string menu_filename)
{
	EnvironmentMenuHelper::SaveMenu(menu_filename);
}
//================================================================================================================
void MenuEditorSystem::AddButton()
{
	// Will be added based on button press, just like add tile

	if (editType == ET_Button && action == A_Place)
	{
		ZShadeSandboxGraphics::Button* b = new ZShadeSandboxGraphics::Button(m_D3DSystem, m_mainGameDirectory, m_EngineOptions,
			ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer("Button"), false,
			m_SelectedMousePos.x, m_SelectedMousePos.y,
			80, 30,
			m_mainGameDirectory->m_menu_sprites_path,
			m_SelectedButtonImageName,
			m_SelectedButtonImageName,
			m_SelectedButtonImageName,
			m_SelectedButtonImageName,
			"white.png",
			"blue.png",
			"yellow.png",
			"black.png");

		m_Updated = true;

		EnvironmentMenuHelper::AddMenuButton(m_ActiveMenuName, b);
	}
}
//================================================================================================================
void MenuEditorSystem::AddText()
{
	if (editType == ET_Text && action == A_Place)
	{
		ZShadeSandboxGraphics::Text* t = new ZShadeSandboxGraphics::Text();
		t->SetD3D(m_D3DSystem);
		t->Initialize();
		t->TextName() = BetterString("Text");
		t->CreateShadow();
		t->ChangeFont(L"Times New Roman", 12, FontStyle::BoldItalic, true);
		t->TextTransform() = XMMatrixTranslation(m_SelectedMousePos.x, m_SelectedMousePos.y, 0);
		t->TextColor() = XMFLOAT4(1, 0, 0, 1);

		m_Updated = true;

		EnvironmentMenuHelper::AddMenuText(m_ActiveMenuName, t);
	}
}
//================================================================================================================
void MenuEditorSystem::AddMenuBackground()
{
	ZShadeSandboxGraphics::Image* image = new ZShadeSandboxGraphics::Image("TitleScreen.png", m_mainGameDirectory->m_sprites_path, 0, 0, 1366, 768, m_D3DSystem);
	EnvironmentMenuHelper::AddMenuBackground(m_ActiveMenuName, image);
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeNone()
{
	editType = ET_None;
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeButton()
{
	editType = ET_Button;
}
//================================================================================================================
void MenuEditorSystem::SwitchToEditTypeText()
{
	editType = ET_Text;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionNone()
{
	action = A_None;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionMove()
{
	action = A_Move;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionDelete()
{
	action = A_Delete;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionPlace()
{
	action = A_Place;
}
//================================================================================================================
void MenuEditorSystem::SwitchToActionEditSize()
{
	action = A_EditSize;
}
//================================================================================================================
void MenuEditorSystem::SwitchToGridModeFree()
{
	gridMode = GM_Free;
}
//================================================================================================================
void MenuEditorSystem::SwitchToGridModeSnap()
{
	gridMode = GM_Snap;
}
//================================================================================================================
