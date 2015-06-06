#include "HUDEditorSystem.h"
#include "Convert.h"
#include "EnvironmentHUDHelper.h"
//================================================================================================================
HUDEditorSystem::HUDEditorSystem(Environment2D* env2D)
:   m_Env2D(env2D)
{
	m_mainGameDirectory = m_Env2D->GetGD2D();
	m_D3DSystem = m_Env2D->GetD3DSystem();
	m_EngineOptions = m_Env2D->GetEngineOptions();

	m_ClickDelayTimer = new SleepTimer();
	m_SelectionTimer = new SleepTimer();
	
	m_SelectedImageImageName = "";
	
	// Set defaults
	SwitchToEditTypeNone();

	m_leftButtonDown = false;
}
//================================================================================================================
HUDEditorSystem::~HUDEditorSystem()
{
}
//================================================================================================================
void HUDEditorSystem::GoFullscreen(bool bEnable)
{
	m_Env2D->GoFullscreen(bEnable);
}
//================================================================================================================
void HUDEditorSystem::LoadRequiredSprites()
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
void HUDEditorSystem::UpdateTitleBar()
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
void HUDEditorSystem::SetGameDirectory(GameDirectory2D* gd)
{
	m_mainGameDirectory = gd;
	m_Env2D->SetGameDirectory(gd);
}
//================================================================================================================
void HUDEditorSystem::UpdateInput(Keyboard* keyboard)
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
void HUDEditorSystem::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_leftButtonDown = true;
		
		m_SelectedMousePos.x = x;
		m_SelectedMousePos.y = y;
		
		// If applicable, Add a button
		if (BetterString(m_SelectedImageImageName) != "")
		{
			AddImage();
		}
		
		// If applicable, Add a text
		AddText();
	}
}
//================================================================================================================
void HUDEditorSystem::OnMouseUp(WPARAM btnState, int x, int y)
{
	// Stop dragging of item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_leftButtonDown = false;
	}
}
//================================================================================================================
void HUDEditorSystem::OnMouseMove(WPARAM btnState, int x, int y)
{
	// Start a left button drag of an item
	if ((btnState & MK_LBUTTON) != 0)
	{
		m_SelectedMousePos.x = x;
		m_SelectedMousePos.y = y;
		
		m_leftButtonDown = true;
		
		// If applicable, Add a button
		if (BetterString(m_SelectedImageImageName) != "")
		{
			AddImage();
		}
		
		// If applicable, Add a text
		AddText();
	}
	else
	{
		m_leftButtonDown = false;
	}
	
	// Move a button or text display cover
	/*if (editMode == ET_Button && (action == A_Place || action == A_Move))
	{
		XMFLOAT2 selToolPoint = SnapToGrid(x, y);
		
		m_StampNormal->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
		m_StampHighlight->TopLeftPosition() = XMFLOAT3(selToolPoint.x, selToolPoint.y, 0);
	}*/
}
//================================================================================================================
XMFLOAT2 HUDEditorSystem::SnapToGrid(int x, int y)
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
void HUDEditorSystem::Render(Camera* camera)
{
	// Can render display cover sprites here for buttons and text
	
	
	
	/*if (m_InSectionPicker)
	{
		m_D3DSystem->TurnOnAlphaBlending();
		{
			if (stampPressed)
				m_StampHighlight32x32->Render11(textureShader, camera);
				//m_StampHighlight32x32->RenderTransparent11(transparentShader, camera, 0);
			else
				m_StampNormal32x32->Render11(textureShader, camera);
				//m_StampNormal32x32->RenderTransparent11(transparentShader, camera, 0);
		}
		m_D3DSystem->TurnOffAlphaBlending();
	}
	else
	{
		if (editMode == EM_Tiles || editMode == EM_Hardness)
		{
			m_D3DSystem->TurnOnAlphaBlending();
			{
				if (stampPressed2)
					m_StampHighlight->Render11(textureShader, camera);
					//m_StampHighlight->RenderTransparent11(transparentShader, camera, 0);
				else
					m_StampNormal->Render11(textureShader, camera);
					//m_StampNormal->RenderTransparent11(transparentShader, camera, 0);
			}
			m_D3DSystem->TurnOffAlphaBlending();
		}
	}*/
}
//================================================================================================================
//void HUDEditorSystem::GrabHUDNames(vector<string>& names)
//{
//	EnvironmentHUDHelper::GrabHUDNames(names);
//}
//================================================================================================================
//void HUDEditorSystem::SetActiveRenderedHUD(string hud_name)
//{
//	EnvironmentHUDHelper::SetActiveRenderedHUD(hud_name);
//}
//================================================================================================================
//void HUDEditorSystem::RemoveHUD(string hud_name)
//{
//	EnvironmentHUDHelper::RemoveHUD(hud_name);
//}
//================================================================================================================
void HUDEditorSystem::CreateHUD(string hud_name)
{
	EnvironmentHUDHelper::CreateHUD(hud_name);
}
//================================================================================================================
void HUDEditorSystem::LoadHUD(string hud_filename, string& hud_name)
{
	EnvironmentHUDHelper::LoadHUD(hud_filename, hud_name);
}
//================================================================================================================
void HUDEditorSystem::SaveHUD(string hud_filename)
{
	EnvironmentHUDHelper::SaveHUD(hud_filename);
}
//================================================================================================================
void HUDEditorSystem::AddImage()
{
	// Will be added based on button press, just like add tile
	
	if (editType == ET_Image && action == A_Place)
	{
		ZShadeSandboxGraphics::Image* i = new ZShadeSandboxGraphics::Image(
			m_SelectedImageImageName,
			m_mainGameDirectory->m_hud_path,
			m_SelectedMousePos.x, m_SelectedMousePos.y,
			100, 100, // Need to get the width and height of the image
			m_D3DSystem,
			m_mainGameDirectory->m_required_textures,
			"white.png"
		);
		
		m_Updated = true;
		
		EnvironmentHUDHelper::AddHUDImage(i);
	}
}
//================================================================================================================
void HUDEditorSystem::AddText()
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

		EnvironmentHUDHelper::AddHUDText(t);
	}
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeNone()
{
	editType = ET_None;
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeImage()
{
	editType = ET_Image;
}
//================================================================================================================
void HUDEditorSystem::SwitchToEditTypeText()
{
	editType = ET_Text;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionNone()
{
	action = A_None;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionMove()
{
	action = A_Move;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionDelete()
{
	action = A_Delete;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionPlace()
{
	action = A_Place;
}
//================================================================================================================
void HUDEditorSystem::SwitchToActionEditSize()
{
	action = A_EditSize;
}
//================================================================================================================
void HUDEditorSystem::SwitchToGridModeFree()
{
	gridMode = GM_Free;
}
//================================================================================================================
void HUDEditorSystem::SwitchToGridModeSnap()
{
	gridMode = GM_Snap;
}
//================================================================================================================
