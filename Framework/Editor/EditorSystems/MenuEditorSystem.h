//================================================================================================================
// MenuEditorSystem.h
//
// Includes everything needed for the menu editor
//================================================================================================================
// History
//
// Created on 2/24/2015 by Dustin Watson
//================================================================================================================
#ifndef __MENUEDITORSYSTEM_H
#define __MENUEDITORSYSTEM_H
//================================================================================================================
//================================================================================================================
//#include "EditorEnvironment.h"
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
//#include "OpenFileDialog.h"
//#include "SaveFileDialog.h"
#include "Environment2D.h"
#include "QuadMesh.h"
//================================================================================================================
class MenuEditorSystem
{
	typedef enum
	{
		ET_None = 0,
		ET_Button = 1,
		ET_Text = 2
	} EEditType;
	
	// Actions to take for a button or text item
	typedef enum
	{
		A_None = 0,
		A_Move = 1,
		A_Delete = 2,
		A_Place = 3,
		A_EditSize = 4// Not for text item
	} EActions;
	
	typedef enum
	{
		GM_Free = 0,
		GM_Snap = 1
	} EGridMode;
	
public:

	MenuEditorSystem(Environment2D* env2D);
	~MenuEditorSystem();

	void Init();

public:
	
	void AddButton();
	void AddText();
	void AddMenuBackground();
	
	void LoadAMenu(string menu_filename, string& menuName);
	void SaveMenu(string menu_filename);
	void CreateMenu(string menu_name);
	void SetActiveRenderedMenu(string menu_name);
	void RemoveMenu(string menu_name);
	void GrabMenuNames(vector<string>& names);

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	void Render(Camera* camera);
	
	void SwitchToEditTypeNone();
	void SwitchToEditTypeButton();
	void SwitchToEditTypeText();
	void SwitchToActionNone();
	void SwitchToActionMove();
	void SwitchToActionDelete();
	void SwitchToActionPlace();
	void SwitchToActionEditSize();
	void SwitchToGridModeFree();
	void SwitchToGridModeSnap();
	
	// Update the text in the title bar of the base window
	void UpdateTitleBar();
	
	XMFLOAT2 SnapToGrid(int x, int y);
	void LoadRequiredSprites();
	void SetGameDirectory(GameDirectory2D* gd);
	void GoFullscreen(bool bEnable);
	GameDirectory2D* GetGD() { return m_mainGameDirectory; }
	void SetGD(GameDirectory2D* gd) { m_mainGameDirectory = gd; }
	D3D* D3DSystem() { return m_D3DSystem; }

	void UpdateInput(Keyboard* keyboard);

	string& SelectedButtonImageName() { return m_SelectedButtonImageName; }
	string SelectedButtonImageName() const { return m_SelectedButtonImageName; }

	string& ActiveMenuName() { return m_ActiveMenuName; }
	string ActiveMenuName() const { return m_ActiveMenuName; }

private:
	
	Environment2D* m_Env2D;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	GameDirectory2D* m_mainGameDirectory;

	string m_ActiveMenuName;

	string m_SelectedButtonImageName;
	
	//Controls how fast the camera will move when transforming
	float m_camera_speed;
	
	//When the user makes a change to the menu then this flag becomes true
	bool m_Updated;
	
	// Display cover for a button image
	Sprite* m_displaySpriteCover;
	//ZShadeSandboxMesh::QuadMesh* m_displaySpriteCover;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	XMFLOAT2 m_SelectedMousePos;
	XMFLOAT2 m_MouseMovePos;
	ZShadeSandboxGraphics::Text mousePosText;
	XMMATRIX transform;
	
	EEditType editType;
	EActions action;
	EGridMode gridMode;
	
	bool m_leftButtonDown;

	//Is the menu created
	bool m_MenuCreated;
	
	bool usingGrid;
	
	bool m_startDelayTimer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;

	//OpenFileDialog* m_MapOpenFileDialog;
	//SaveFileDialog* m_MapSaveFileDialog;
};
//================================================================================================================
#endif//__MENUEDITORSYSTEM_H