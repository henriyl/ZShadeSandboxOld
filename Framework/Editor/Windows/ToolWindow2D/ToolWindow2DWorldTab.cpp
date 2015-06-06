#include "ToolWindow2DWorldTab.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DWorldTab::ToolWindow2DWorldTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
,   m_IsPlatformer(false)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DWorldTab::~ToolWindow2DWorldTab()
{

}
//================================================================================================================
void ToolWindow2DWorldTab::BuildTab()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_hwnd;

	m_WorldName.Init(hwnd, "No world Created", 150, 70, 200, 30, EEditorTypeID::eWorldTabWorldCreatedLabel);

	createWorldNamelabel.Init(hwnd, "World Name", x, y, 100, 30, EEditorTypeID::eWorldTabNameLabel);
	m_CreateWorldName.Init(hwnd, x + 110, y, 100, 20, EEditorTypeID::eWorldTabNameLabel2);
	y += 40;
	m_CreateWorld.Init(hwnd, "Create World", x, y, 100, 30, EEditorTypeID::eWorldTabCreateButton);
	y += 40;
	m_LoadWorld.Init(hwnd, "Load World", x, y, 100, 30, EEditorTypeID::eWorldTabLoadButton);
	m_SaveWorld.Init(hwnd, "Save World", x + 110, y, 100, 30, EEditorTypeID::eWorldTabSaveButton);

	// Create new map area for a topdown map
	if (!m_IsPlatformer)
	{
		y += 120;

		int yy = y;

		m_TopdownMapSizeLabel.Init(hwnd, "Map Size", x, y, 100, 30, EEditorTypeID::eWorldTabMapSizeLabel);
		m_TopdownMapSizeCombobox.Init(hwnd, x + 100, y, 80, 500, EEditorTypeID::eWorldTabMapSizeComboBox);
		m_TopdownMapSizeCombobox.AddItem("1x1");
		m_TopdownMapSizeCombobox.AddItem("3x3");
		m_TopdownMapSizeCombobox.AddItem("6x6");
		m_TopdownMapSizeCombobox.AddItem("8x8");
		m_TopdownMapSizeCombobox.AddItem("10x10");
		m_TopdownMapSizeCombobox.AddItem("20x20");
		m_TopdownMapSizeCombobox.AddItem("30x30");
		m_TopdownMapSizeCombobox.SetDisplayItem("1x1", 0);

		y += 30;
		m_TopdownIsoStandLabel.Init(hwnd, "Map Type", x, y, 100, 30, EEditorTypeID::eWorldTabMapIsoStandLabel);
		y += 15;
		m_TopdownIsoStandRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eWorldTabMapIsoStandRadioContainer);
		m_TopdownIsoStandRadioContainer.AddRadioButton(hwnd, true, "STANDARD", x, y, 100, 30, EEditorTypeID::eWorldTabMapStandardRadioButton);
		m_TopdownIsoStandRadioContainer.AddRadioButton(hwnd, false, "ISOMETRIC", x + 100, y, 100, 30, EEditorTypeID::eWorldTabMapIsoRadioButton);
		SendMessage(m_TopdownIsoStandRadioContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

		y += 65;
		m_TopdownWorldNameLabel.Init(hwnd, "World Name", x, y, 100, 30, EEditorTypeID::eWorldTabWorldNameLabel);
		m_TopdownWorldNameComboBox.Init(hwnd, x + 110, y, 80, 500, EEditorTypeID::eWorldTabWorldNameComboBox);

		y += 30;
		m_TopdownMapNameLabel.Init(hwnd, "Map Name", x, y, 100, 30, EEditorTypeID::eWorldTabMapNameLabel);
		m_TopdownMapNameTextbox.Init(hwnd, x + 110, y, 100, 20, EEditorTypeID::eWorldTabMapNameTextbox);

		int yyy = y;

		y = yy;
		x = 270;
		m_TopdownFowEnabledLabel.Init(hwnd, "Enable FOW", x, y, 100, 30, EEditorTypeID::eWorldTabMapFowEnabledLabel);
		y += 15;
		m_TopdownFowEnabledRadioContainer.Init(hwnd, x, y, 100, 50, EEditorTypeID::eWorldTabMapFowEnabledRadioContainer);
		m_TopdownFowEnabledRadioContainer.AddRadioButton(hwnd, true, "Yes", x, y, 50, 50, EEditorTypeID::eWorldTabMapFowEnabledYes);
		m_TopdownFowEnabledRadioContainer.AddRadioButton(hwnd, false, "No", x + 50, y, 50, 50, EEditorTypeID::eWorldTabMapFowEnabledNo);
		SendMessage(m_TopdownFowEnabledRadioContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

		y += 70;
		m_TopdownFowFlashlightEnabled.Init(hwnd, "FOW Flashlight", x, y, 130, 20, EEditorTypeID::eWorldTabFowFlashlight);
		m_TopdownFowFlashlightEnabled.Check(false);
		EnableWindow(m_TopdownFowFlashlightEnabled.GetHwnd(), false);

		y += 20;
		m_TopdownFowRadiusLabel.Init(hwnd, "FOW Radius", x, y + 5, 80, 30, EEditorTypeID::eWorldTabMapFowRadiusLabel);
		m_TopdownFowRadiusComboBox.Init(hwnd, x, y + 20, 80, 500, EEditorTypeID::eWorldTabMapFowRadiusComboBox);
		m_TopdownFowRadiusComboBox.AddItem("3");
		m_TopdownFowRadiusComboBox.AddItem("4");
		m_TopdownFowRadiusComboBox.AddItem("5");
		m_TopdownFowRadiusComboBox.AddItem("6");
		m_TopdownFowRadiusComboBox.AddItem("7");
		m_TopdownFowRadiusComboBox.AddItem("8");
		m_TopdownFowRadiusComboBox.AddItem("9");
		m_TopdownFowRadiusComboBox.SetDisplayItem("3", 0);
		EnableWindow(m_TopdownFowRadiusComboBox.GetHwnd(), false);

		x = 30;
		y = yyy;
		y += 40;
		m_createTopdownMapButton.Init(hwnd, "Create Map", x, y, 100, 30, EEditorTypeID::eWorldTabCreateTopdownMapButton);
	}

	// Create new map area for a platformer map
	if (m_IsPlatformer)
	{
		y += 120;

		//y = 70;
		m_PlatformerMapWidthLabel.Init(hwnd, "Width", x, y, 100, 30, EEditorTypeID::eWorldTabMapWidthLabel);
		m_PlatformerMapWidthTextbox.Init(hwnd, x + 110, y, 100, 20, EEditorTypeID::eWorldTabMapWidthTextbox);
		
		y += 30;
		m_PlatformerMapHeightLabel.Init(hwnd, "Height", x, y, 100, 30, EEditorTypeID::eWorldTabMapHeightLabel);
		m_PlatformerMapHeightTextbox.Init(hwnd, x + 110, y, 100, 20, EEditorTypeID::eWorldTabMapHeightTextbox);

		y += 30;
		m_PlatformerWorldNameLabel.Init(hwnd, "World Name", x, y + 5, 100, 30, EEditorTypeID::eWorldTabPlatformerWorldNameLabel);
		m_PlatformerWorldNameComboBox.Init(hwnd, x + 110, y, 80, 500, EEditorTypeID::eWorldTabPlatformerWorldNameListComboBox);

		y += 35;
		m_PlatformerMapNameLabel.Init(hwnd, "Map Name", x, y, 100, 30, EEditorTypeID::eWorldTabPlatformerMapNameLabel);
		m_PlatformerMapNameTextbox.Init(hwnd, x + 110, y, 100, 20, EEditorTypeID::eWorldTabPlatformerMapNameTextbox);

		x = 30;
		y += 40;
		m_createPlatformerMapButton.Init(hwnd, "Create Map", x, y, 100, 30, EEditorTypeID::eWorldTabCreatePlatformerMapButton);
	}

	//y += 40;
	//m_CreateMap.Init(hwnd, "Create Map", x, y, 100, 30, CREATE_MAP);
	y += 90;
	m_LoadMap.Init(hwnd, "Load Map", x, y, 100, 30, EEditorTypeID::eWorldTabLoadMapButton);
	m_SaveMap.Init(hwnd, "Save Map", x + 110, y, 100, 30, EEditorTypeID::eWorldTabSaveMapButton);

	y += 60;

	m_WTMapNameLabel.Init(hwnd, "Map Name", x, y, 100, 30, EEditorTypeID::eWorldTabWTMapNameLabel);
	m_MapNameList.Init(hwnd, x + 100, y, 150, 200, EEditorTypeID::eWorldTabWTMapNameComboBox);
	y += 30;
	m_RenderedMap.Init(hwnd, "Render Map", x, y, 100, 30, EEditorTypeID::eWorldTabMapRenderedButton);
	m_RemoveMap.Init(hwnd, "Remove Map", x + 110, y, 100, 30, EEditorTypeID::eWorldTabMapRemoveButton);

	m_createPlatformerMapButton.SetVisible(false);
	m_PlatformerMapWidthLabel.SetVisible(false);
	m_PlatformerMapWidthTextbox.SetVisible(false);
	m_PlatformerMapHeightLabel.SetVisible(false);
	m_PlatformerMapHeightTextbox.SetVisible(false);
	m_PlatformerMapNameLabel.SetVisible(false);
	m_PlatformerMapNameTextbox.SetVisible(false);
	m_PlatformerWorldNameLabel.SetVisible(false);
	m_PlatformerWorldNameComboBox.SetVisible(false);
}
//================================================================================================================
void ToolWindow2DWorldTab::VisibleTabItems(bool visible)
{
	createWorldNamelabel.SetVisible(visible);
	m_CreateWorldName.SetVisible(visible);
	m_CreateWorld.SetVisible(visible);
	m_LoadWorld.SetVisible(visible);
	m_SaveWorld.SetVisible(visible);
	m_CreateMap.SetVisible(visible);
	m_LoadMap.SetVisible(visible);
	m_SaveMap.SetVisible(visible);
	m_RenderedMap.SetVisible(visible);
	m_RemoveMap.SetVisible(visible);
	m_WorldName.SetVisible(visible);
	m_WTMapNameLabel.SetVisible(visible);
	m_MapNameList.SetVisible(visible);

	if (!m_IsPlatformer)
	{
		m_createTopdownMapButton.SetVisible(visible);
		m_TopdownMapSizeLabel.SetVisible(visible);
		m_TopdownMapSizeCombobox.SetVisible(visible);
		m_TopdownIsoStandLabel.SetVisible(visible);
		m_TopdownIsoStandRadioContainer.SetVisible(visible);
		m_TopdownMapNameLabel.SetVisible(visible);
		m_TopdownMapNameTextbox.SetVisible(visible);
		m_TopdownWorldNameLabel.SetVisible(visible);
		m_TopdownWorldNameComboBox.SetVisible(visible);
		m_TopdownFowEnabledLabel.SetVisible(visible);
		m_TopdownFowEnabledRadioContainer.SetVisible(visible);
		m_TopdownFowRadiusLabel.SetVisible(visible);
		m_TopdownFowRadiusComboBox.SetVisible(visible);
		m_TopdownFowFlashlightEnabled.SetVisible(visible);
	}
	else
	{
		m_createPlatformerMapButton.SetVisible(visible);
		m_PlatformerMapWidthLabel.SetVisible(visible);
		m_PlatformerMapWidthTextbox.SetVisible(visible);
		m_PlatformerMapHeightLabel.SetVisible(visible);
		m_PlatformerMapHeightTextbox.SetVisible(visible);
		m_PlatformerMapNameLabel.SetVisible(visible);
		m_PlatformerMapNameTextbox.SetVisible(visible);
		m_PlatformerWorldNameLabel.SetVisible(visible);
		m_PlatformerWorldNameComboBox.SetVisible(visible);
	}
}
//================================================================================================================
void ToolWindow2DWorldTab::EnableTabItems(bool enable)
{
	createWorldNamelabel.Enable(enable);
	m_CreateWorldName.Enable(enable);
	m_CreateWorld.Enable(enable);
	m_LoadWorld.Enable(enable);
	m_SaveWorld.Enable(enable);
	m_CreateMap.Enable(enable);
	m_LoadMap.Enable(enable);
	m_SaveMap.Enable(enable);
	m_RenderedMap.Enable(enable);
	m_RemoveMap.Enable(enable);
	m_WorldName.Enable(enable);
	m_WTMapNameLabel.Enable(enable);
	m_MapNameList.Enable(enable);

	if (!m_IsPlatformer)
	{
		m_createTopdownMapButton.Enable(enable);
		m_TopdownMapSizeLabel.Enable(enable);
		m_TopdownMapSizeCombobox.Enable(enable);
		m_TopdownIsoStandLabel.Enable(enable);
		m_TopdownIsoStandRadioContainer.Enable(enable);
		m_TopdownMapNameLabel.Enable(enable);
		m_TopdownMapNameTextbox.Enable(enable);
		m_TopdownWorldNameLabel.Enable(enable);
		m_TopdownWorldNameComboBox.Enable(enable);
		m_TopdownFowEnabledLabel.Enable(enable);
		m_TopdownFowEnabledRadioContainer.Enable(enable);
		m_TopdownFowRadiusLabel.Enable(enable);
		m_TopdownFowRadiusComboBox.Enable(enable);
		m_TopdownFowFlashlightEnabled.Enable(enable);
	}
	else
	{
		m_createPlatformerMapButton.Enable(enable);
		m_PlatformerMapWidthLabel.Enable(enable);
		m_PlatformerMapWidthTextbox.Enable(enable);
		m_PlatformerMapHeightLabel.Enable(enable);
		m_PlatformerMapHeightTextbox.Enable(enable);
		m_PlatformerMapNameLabel.Enable(enable);
		m_PlatformerMapNameTextbox.Enable(enable);
		m_PlatformerWorldNameLabel.Enable(enable);
		m_PlatformerWorldNameComboBox.Enable(enable);
	}
}
//================================================================================================================
void ToolWindow2DWorldTab::Update()
{

}
//================================================================================================================
LRESULT ToolWindow2DWorldTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	return true;
}
//================================================================================================================