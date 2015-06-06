#include "ToolWindowMenu.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindowMenu::ToolWindowMenu(EngineOptions* eo, D3D* d3d, Environment2D* env2D, Environment3D* env3D, SplitterWindow* editWindow)
:   m_Env2DRef(env2D)
,   m_Env3DRef(env3D)
,   m_EngineOptions(eo)
,   m_D3DSystem(d3d)
,   m_BaseEditWindowRef(editWindow)
{
	Init();
}
//================================================================================================================
ToolWindowMenu::~ToolWindowMenu()
{

}
//================================================================================================================
void ToolWindowMenu::Init()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_BaseEditWindowRef->GetHwndRight();

	m_MenuItemListbox.Init(hwnd, x, y, 200, 400, EEditorTypeID::eMenuItemListbox);

	y += 400;

	m_SelectMenuItemText.Init(hwnd, "Selected Menu Item: ", x, y, 130, 30, EEditorTypeID::eMenuItemSelectedLabel);
	m_SelectMenuItemText2.Init(hwnd, "", x + 110, y, 150, 30, EEditorTypeID::eMenuItemSelectedLabel2);

	y += 40;

	m_SelectMenuButton.Init(hwnd, "Select Menu Item", x, y, 170, 30, EEditorTypeID::eMenuItemSelectButton);
	m_UpdateMenuButton.Init(hwnd, "Update", x + 180, y, 120, 30, EEditorTypeID::eMenuItemUpdateButton);

	//y += 70;
	x += 250;
	y = 30;
	m_MenuItemText.Init(hwnd, "Menu Item", x, y, 100, 30, EEditorTypeID::eMenuItemLabel);
	y += 10;
	m_MenuItemContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eMenuItemLabelRadioContainer);// 120, 110
	m_MenuItemContainer.AddRadioButton(hwnd, false, "Button", x + 10, y + 10, 100, 30, EEditorTypeID::eMenuItemButton);
	m_MenuItemContainer.AddRadioButton(hwnd, false, "Text", x + 10, y + 40, 100, 30, EEditorTypeID::eMenuItemText);
	m_MenuItemContainer.AddRadioButton(hwnd, true, "None", x + 10, y + 70, 100, 30, EEditorTypeID::eMenuItemNone);
	SendMessage(m_MenuItemContainer.GetButtonHandle(2), BM_SETCHECK, BST_CHECKED, 0);
	
	// Creating a new menu
	{
		x = 10;
		y = 30;
		
		int textWidth = 100;
		int textboxXDiff = textWidth + 10;
		int textboxWidth = 100;
		
		m_menuNameText.Init(hwnd, "Menu Name", x, y, textWidth, 30, EEditorTypeID::eMenuNewNameText);
		m_menuNameTextbox.Init(hwnd, x + textboxXDiff, y, textboxWidth, 20, EEditorTypeID::eMenuNewNameTextbox);
		m_menuNameTextbox.SetText("NONE");
		
		int radioButtonWidth = 100;
		int radioWidth = radioButtonWidth * 3;
		int radioHeight = 25;
		
		y += 50;
		m_menuSizeText.Init(hwnd, "Menu Size", x, y, textWidth, 30, EEditorTypeID::eMenuNewSizeText);
		m_menuSizeContainer.Init(hwnd, x, y + 17, 0, 0, EEditorTypeID::eMenuNewSizeRadioContainer);
		m_menuSizeContainer.AddRadioButton(hwnd, true, "800x600", x, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eMenuNewSizeRadio800x600);
		m_menuSizeContainer.AddRadioButton(hwnd, false, "1024x768", x + 100, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eMenuNewSizeRadio1024x768);
		m_menuSizeContainer.AddRadioButton(hwnd, false, "1366x768", x + 200, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eMenuNewSizeRadio1366x768);
		m_menuSizeContainer.AddRadioButton(hwnd, false, "1280x1024", x, y + 17 + radioHeight, radioButtonWidth, radioHeight, EEditorTypeID::eMenuNewSizeRadio1280x1024);
		m_menuSizeContainer.AddRadioButton(hwnd, false, "1920x1080", x + 100, y + 17 + radioHeight, radioButtonWidth * 2, radioHeight, EEditorTypeID::eMenuNewSizeRadio1920x1080);
		SendMessage(m_menuSizeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
		
		radioButtonWidth = 100;
		radioWidth = radioButtonWidth * 2;
		
		y += 80;
		m_menuTypeText.Init(hwnd, "Menu Type", x, y, textWidth, 30, EEditorTypeID::eMenuNewTypeText);
		m_menuTypeContainer.Init(hwnd, x, y + 17, 0, 0, EEditorTypeID::eMenuNewTypeRadioContainer);
		m_menuTypeContainer.AddRadioButton(hwnd, true, "Main Menu", x, y + 17, radioButtonWidth, radioHeight, EEditorTypeID::eMenuNewTypeRadioMainMenu);
		m_menuTypeContainer.AddRadioButton(hwnd, false, "In Game Menu", x + 100, y + 17, radioButtonWidth + 30, radioHeight, EEditorTypeID::eMenuNewTypeRadioInGameMenu);
		SendMessage(m_menuTypeContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
		
		m_createButton.Init(hwnd, "Create Menu", 295, 160, 100, 30, EEditorTypeID::eMenuNewCreateButton);
	}
	
	x = 400;
	y = 40;
	//m_CreateMenu.Init(hwnd, "Create Menu", x, y, 100, 30, EEditorTypeID::eMenuCreateButton);
	y += 40;
	m_LoadMenu.Init(hwnd, "Load Menu", x, y, 100, 30, EEditorTypeID::eMenuLoadButton);
	m_SaveMenu.Init(hwnd, "Save Menu", x + 110, y, 100, 30, EEditorTypeID::eMenuSaveButton);

	y += 60;
	m_MenuListText.Init(hwnd, "Menu Name", x, y, 100, 30, EEditorTypeID::eMenuNameListLabel);
	m_MenuNameList.Init(hwnd, x + 100, y, 150, 200, EEditorTypeID::eMenuNameListComboBox);

	y += 30;
	m_RenderedMenu.Init(hwnd, "Render Menu", x, y, 100, 30, EEditorTypeID::eMenuRenderedButton);
	m_RemoveMenu.Init(hwnd, "Remove Menu", x + 110, y, 100, 30, EEditorTypeID::eMenuRemoveButton);

	y += 30;
	m_AddMenuBackground.Init(hwnd, "Add Background", x, y, 100, 30, EEditorTypeID::eMenuAddBackground);

	y += 30;
	m_ToMapEditorButton.Init(hwnd, "To Map Editor", x, y, 100, 30, EEditorTypeID::eMenuMapEditorButton);

	bMenuItemNone = true;
	bMenuItemButton = false;
	bMenuItemText = false;
	bHideWindow = false;
}
//================================================================================================================
bool ToolWindowMenu::DoHideWindow()
{
	return bHideWindow;
}
//================================================================================================================
void ToolWindowMenu::SetDoHideWindow(bool set)
{
	bHideWindow = set;
}
//================================================================================================================
void ToolWindowMenu::ShowWindow()
{
	ShowItems(true);
	EnableItems(true);
}
//================================================================================================================
void ToolWindowMenu::HideWindow()
{
	ShowItems(false);
	EnableItems(false);
}
//================================================================================================================
void ToolWindowMenu::ShowItems(bool visible)
{
	m_SelectMenuItemText.SetVisible(visible);
	m_SelectMenuItemText2.SetVisible(visible);
	m_SelectMenuButton.SetVisible(visible);
	m_UpdateMenuButton.SetVisible(visible);
	m_MenuItemListbox.SetVisible(visible);
	m_MenuItemText.SetVisible(visible);
	m_MenuItemContainer.SetVisible(visible);
	m_LoadMenu.SetVisible(visible);
	m_SaveMenu.SetVisible(visible);
	m_RenderedMenu.SetVisible(visible);
	m_RemoveMenu.SetVisible(visible);
	m_MenuListText.SetVisible(visible);
	m_MenuNameList.SetVisible(visible);
	m_AddMenuBackground.SetVisible(visible);
	m_ToMapEditorButton.SetVisible(visible);
	m_createButton.SetVisible(visible);
	m_menuSizeText.SetVisible(visible);
	m_menuNameText.SetVisible(visible);
	m_menuTypeText.SetVisible(visible);
	m_menuSizeContainer.SetVisible(visible);
	m_menuTypeContainer.SetVisible(visible);
	m_menuNameTextbox.SetVisible(visible);
}
//================================================================================================================
void ToolWindowMenu::EnableItems(bool enable)
{
	m_SelectMenuItemText.Enable(enable);
	m_SelectMenuItemText2.Enable(enable);
	m_SelectMenuButton.Enable(enable);
	m_UpdateMenuButton.Enable(enable);
	m_MenuItemListbox.Enable(enable);
	m_MenuItemText.Enable(enable);
	m_MenuItemContainer.Enable(enable);
	m_LoadMenu.Enable(enable);
	m_SaveMenu.Enable(enable);
	m_RenderedMenu.Enable(enable);
	m_RemoveMenu.Enable(enable);
	m_MenuListText.Enable(enable);
	m_MenuNameList.Enable(enable);
	m_AddMenuBackground.Enable(enable);
	m_ToMapEditorButton.Enable(enable);
	m_createButton.Enable(enable);
	m_menuSizeText.Enable(enable);
	m_menuNameText.Enable(enable);
	m_menuTypeText.Enable(enable);
	m_menuSizeContainer.Enable(enable);
	m_menuTypeContainer.Enable(enable);
	m_menuNameTextbox.Enable(enable);
}
//================================================================================================================
void ToolWindowMenu::Update()
{

}
//================================================================================================================
void ToolWindowMenu::FillCache()
{

}
//================================================================================================================
LRESULT ToolWindowMenu::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
//================================================================================================================