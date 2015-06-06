//================================================================================================================
// ImageSampleWindow.h
//
//================================================================================================================
// History
//
// Created on 1/11/2014 by Dustin Watson
//================================================================================================================
#ifndef __IMAGESAMPLEWINDOW_H
#define __IMAGESAMPLEWINDOW_H
//================================================================================================================
//================================================================================================================
// Includes
#include "D3D.h"
#include "Camera.h"
#include "Sprite.h"
#include "Window.h"
#include "TextureShader.h"
#include "GameDirectory.h"

//================================================================================================================

class ImageSampleWindow
{
public:

	ImageSampleWindow() {}
	ImageSampleWindow(HINSTANCE hInstance, HWND parent);

	void Init(HINSTANCE hInstance, HWND parent);

	void LoadImageSample(string base_path, string imageName);

	void Invalidate();

	void Show(bool bShow);
	void SetPosition(int x, int y);

	bool IsRunning() { return m_running; }

	bool IsCreated() { return m_created; }
	void SetCreated(bool created) { m_created = created; }

	void SetEscape(bool bSet) { m_escape = bSet; }

	bool InFocus();

private:

	LRESULT Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	Camera* camera;
	Sprite* sprite;
	TextureShader* textureShader;
	Window* m_Window;
	bool bSpriteLoaded;

	string imageName;

	bool m_escape;
	bool m_created;
	bool m_running;
};
//================================================================================================================
//================================================================================================================
#endif//__IMAGESAMPLEWINDOW_H