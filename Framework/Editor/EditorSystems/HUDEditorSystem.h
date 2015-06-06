//================================================================================================================
// HUDEditorSystem.h
//
// Includes everything needed for the HUD editor
//================================================================================================================
// History
//
// Created on 3/4/2015 by Dustin Watson
//================================================================================================================
#ifndef __HUDEDITORSYSTEM_H
#define __HUDEDITORSYSTEM_H
//================================================================================================================
//================================================================================================================
//#include "EditorEnvironment.h"
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
//#include "OpenFileDialog.h"
//#include "SaveFileDialog.h"
#include "Environment2D.h"
//================================================================================================================
class HUDEditorSystem
{
	typedef enum
	{
		ET_None = 0,
		ET_Image = 1,
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
	
	HUDEditorSystem(Environment2D* env2D);
	~HUDEditorSystem();

	void Init();

public:
	
	void AddImage();
	void AddText();
	
	void LoadHUD(string hud_filename, string& hud_name);
	void SaveHUD(string hud_filename);
	void CreateHUD(string hud_name);
	//void SetActiveRenderedHUD(string hud_name);
	//void RemoveHUD(string hud_name);
	//void GrabHUDNames(vector<string>& names);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	void Render(Camera* camera);
	
	void SwitchToEditTypeNone();
	void SwitchToEditTypeImage();
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
	D3D* D3DSystem() { return m_D3DSystem; }
	
	void UpdateInput(Keyboard* keyboard);
	
	string& SelectedImageImageName() { return m_SelectedImageImageName; }
	string SelectedImageImageName() const { return m_SelectedImageImageName; }
	
	//string& ActiveHUDName() { return m_ActiveHUDName; }
	//string ActiveHUDName() const { return m_ActiveHUDName; }
	
private:
	
	Environment2D* m_Env2D;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	GameDirectory2D* m_mainGameDirectory;

	//string m_ActiveHUDName;
	string m_SelectedImageImageName;
	
	//Controls how fast the camera will move when transforming
	float m_camera_speed;
	
	//When the user makes a change to the hud then this flag becomes true
	bool m_Updated;
	
	// Display cover for a button image
	Sprite* m_displaySpriteCover;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	XMFLOAT2 m_SelectedMousePos;
	
	EEditType editType;
	EActions action;
	EGridMode gridMode;
	
	bool m_leftButtonDown;

	//Is the hud created
	bool m_HUDCreated;
	
	bool usingGrid;
	
	bool m_startDelayTimer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;

	//OpenFileDialog* m_MapOpenFileDialog;
	//SaveFileDialog* m_MapSaveFileDialog;
};
//================================================================================================================
#endif//__HUDEDITORSYSTEM_H