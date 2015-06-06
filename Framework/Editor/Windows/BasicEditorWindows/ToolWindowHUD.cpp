#include "ToolWindowHUD.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindowHUD::ToolWindowHUD(EngineOptions* eo, D3D* d3d, Environment2D* env2D, Environment3D* env3D, SplitterWindow* editWindow)
:   m_Env2DRef(env2D)
,   m_Env3DRef(env3D)
,   m_EngineOptions(eo)
,   m_D3DSystem(d3d)
,   m_BaseEditWindowRef(editWindow)
{
	Init();
}
//================================================================================================================
ToolWindowHUD::~ToolWindowHUD()
{

}
//================================================================================================================
void ToolWindowHUD::Init()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_BaseEditWindowRef->GetHwndRight();

	m_HUDItemListbox.Init(hwnd, x, y, 200, 400, EEditorTypeID::eHUDItemListbox);

	y += 400;

	m_SelectHUDItemText.Init(hwnd, "Selected HUD Item: ", x, y, 150, 30, EEditorTypeID::eHUDItemSelectedLabel);
	m_SelectHUDItemText2.Init(hwnd, "", x + 160, y, 150, 30, EEditorTypeID::eHUDItemSelectedLabel2);

	y += 40;

	m_SelectHUDButton.Init(hwnd, "Select HUD Item", x, y, 170, 30, EEditorTypeID::eHUDItemSelectButton);
	m_UpdateHUDButton.Init(hwnd, "Update", x + 180, y, 120, 30, EEditorTypeID::eHUDItemUpdateButton);

	//y += 70;
	x += 250;
	y = 30;
	m_HUDItemText.Init(hwnd, "HUD Item", x, y, 100, 30, EEditorTypeID::eHUDItemLabel);
	y += 10;
	m_HUDItemContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eHUDItemLabelRadioContainer);
	m_HUDItemContainer.AddRadioButton(hwnd, false, "Image", x + 10, y + 10, 100, 30, EEditorTypeID::eHUDItemImage);
	m_HUDItemContainer.AddRadioButton(hwnd, false, "Text", x + 10, y + 40, 100, 30, EEditorTypeID::eHUDItemText);
	m_HUDItemContainer.AddRadioButton(hwnd, true, "None", x + 10, y + 70, 100, 30, EEditorTypeID::eHUDItemNone);
	SendMessage(m_HUDItemContainer.GetButtonHandle(2), BM_SETCHECK, BST_CHECKED, 0);
	
	// Creating a new HUD
	{
		int radioButtonWidth = 100;
		int radioWidth = radioButtonWidth * 3;
		int radioHeight = 25;

		y += 50;
		m_hudSizeText.Init(hwnd, "HUD Size", x, y, 100, 30, EEditorTypeID::eHUDNewSizeText);
		m_hudSizeContainer.Init(hwnd, x, y + 17, 0, 0, EEditorTypeID::eHUDNewSizeRadioContainer);
		m_hudSizeContainer.AddRadioButton(hwnd, true, "800x600", x, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eHUDNewSizeRadio800x600);
		m_hudSizeContainer.AddRadioButton(hwnd, false, "1024x768", x + 100, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eHUDNewSizeRadio1024x768);
		m_hudSizeContainer.AddRadioButton(hwnd, false, "1366x768", x + 200, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eHUDNewSizeRadio1366x768);
		m_hudSizeContainer.AddRadioButton(hwnd, false, "1280x1024", x, y + 17 + radioHeight, radioButtonWidth, radioHeight, EEditorTypeID::eHUDNewSizeRadio1280x1024);
		m_hudSizeContainer.AddRadioButton(hwnd, false, "1920x1080", x + 100, y + 17 + radioHeight, radioButtonWidth * 2, radioHeight, EEditorTypeID::eHUDNewSizeRadio1920x1080);
		SendMessage(m_hudSizeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
		
		x = 295;
		y += 35;
		m_createButton.Init(hwnd, "Create HUD", x, y, 100, 30, EEditorTypeID::eHUDNewCreateButton);
	}
	
	x = 400;
	y = 40;
	//m_CreateHUD.Init(hwnd, "Create HUD", x, y, 100, 30, EEditorTypeID::eHUDCreateButton);
	y += 40;
	m_LoadHUD.Init(hwnd, "Load HUD", x, y, 100, 30, EEditorTypeID::eHUDLoadButton);
	m_SaveHUD.Init(hwnd, "Save HUD", x + 110, y, 100, 30, EEditorTypeID::eHUDSaveButton);

	y += 60;
	m_HUDListText.Init(hwnd, "HUD Name", x, y, 100, 30, EEditorTypeID::eHUDNameListLabel);
	m_HUDNameList.Init(hwnd, x + 100, y, 150, 200, EEditorTypeID::eHUDNameListComboBox);

	y += 30;
	m_RenderedHUD.Init(hwnd, "Render HUD", x, y, 100, 30, EEditorTypeID::eHUDRenderedButton);
	m_RemoveHUD.Init(hwnd, "Remove HUD", x + 110, y, 100, 30, EEditorTypeID::eHUDRemoveButton);

	y += 30;
	m_ToMapEditorButton.Init(hwnd, "To Map Editor", x, y, 100, 30, EEditorTypeID::eHUDMapEditorButton);

	bHUDItemNone = true;
	bHUDItemImage = false;
	bHUDItemText = false;
	bHideWindow = false;
}
//================================================================================================================
bool ToolWindowHUD::DoHideWindow()
{
	return bHideWindow;
}
//================================================================================================================
void ToolWindowHUD::SetDoHideWindow(bool set)
{
	bHideWindow = set;
}
//================================================================================================================
void ToolWindowHUD::ShowWindow()
{
	ShowItems(true);
	EnableItems(true);
}
//================================================================================================================
void ToolWindowHUD::HideWindow()
{
	ShowItems(false);
	EnableItems(false);
}
//================================================================================================================
void ToolWindowHUD::ShowItems(bool visible)
{
	m_SelectHUDItemText.SetVisible(visible);
	m_SelectHUDItemText2.SetVisible(visible);
	m_SelectHUDButton.SetVisible(visible);
	m_UpdateHUDButton.SetVisible(visible);
	m_HUDItemListbox.SetVisible(visible);
	m_HUDItemText.SetVisible(visible);
	m_HUDItemContainer.SetVisible(visible);
	m_LoadHUD.SetVisible(visible);
	m_SaveHUD.SetVisible(visible);
	m_RenderedHUD.SetVisible(visible);
	m_RemoveHUD.SetVisible(visible);
	m_HUDListText.SetVisible(visible);
	m_HUDNameList.SetVisible(visible);
	m_ToMapEditorButton.SetVisible(visible);
	m_createButton.SetVisible(visible);
	m_hudSizeText.SetVisible(visible);
	m_hudSizeContainer.SetVisible(visible);
}
//================================================================================================================
void ToolWindowHUD::EnableItems(bool enable)
{
	m_SelectHUDItemText.Enable(enable);
	m_SelectHUDItemText2.Enable(enable);
	m_SelectHUDButton.Enable(enable);
	m_UpdateHUDButton.Enable(enable);
	m_HUDItemListbox.Enable(enable);
	m_HUDItemText.Enable(enable);
	m_HUDItemContainer.Enable(enable);
	m_LoadHUD.Enable(enable);
	m_SaveHUD.Enable(enable);
	m_RenderedHUD.Enable(enable);
	m_RemoveHUD.Enable(enable);
	m_HUDListText.Enable(enable);
	m_HUDNameList.Enable(enable);
	m_ToMapEditorButton.Enable(enable);
	m_createButton.Enable(enable);
	m_hudSizeText.Enable(enable);
	m_hudSizeContainer.Enable(enable);
}
//================================================================================================================
void ToolWindowHUD::Update()
{

}
//================================================================================================================
void ToolWindowHUD::FillCache()
{

}
//================================================================================================================
LRESULT ToolWindowHUD::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//================================================================================================================