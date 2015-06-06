#ifndef __BUTTON_H
#define __BUTTON_H
//-----------------------------------------------------------------------------------------
#include "D3DIncludes.h"
#include <string>
#include "CGlobal.h"
#include "input.h"
#include "D3D.h"
#include "GameDirectory.h"
#include "Script.h"
using std::string;
//-----------------------------------------------------------------------------------------
#include "Text.h"
#include "Image.h"
//-----------------------------------------------------------------------------------------
namespace ZShadeSandboxGraphics {
	namespace EButtonState
	{
		enum
		{
			eNormal,
			ePushed,
			eHighlighted,
			eDisabled
		};
	}
	//-----------------------------------------------------------------------------------------
	namespace EScriptType
	{
		enum
		{
			eNone,
			eStart,
			eResume,
			eContinue,
			eOptions,
			eExit
		};
	}
	//-----------------------------------------------------------------------------------------
	//An Image/Color with Text,
	//Has 5 states: NORMAL, PUSHED, HIGHLIGHTED, DISABLED, UP

	//Need to detect if mouse has entered the button - Mouse Coord in the bounds of the button
	//And detect if the mouse has clicked on the button
	class Button
	{
	public:
		Button(D3D* d3d, GameDirectory2D* gd, EngineOptions* eo, char* text, bool disableHighlight,
			float x, float y, int width, int height,
			string normalImageName, string pushedImageName, string highlightedImageName, string disabledImageName,
			string pNormalBackgroundColor, string pPushedBackgroundColor, string pHighlightedBackgroundColor, string pDisabledBackgroundColor);
		Button(D3D* d3d, GameDirectory2D* gd, EngineOptions* eo, char* text, bool disableHighlight,
			float x, float y, int width, int height,
			string base_button_path,
			string normalImageName, string pushedImageName, string highlightedImageName, string disabledImageName,
			string pNormalBackgroundColor, string pPushedBackgroundColor, string pHighlightedBackgroundColor, string pDisabledBackgroundColor);
		~Button();

		void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
		void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
		void UpdateButtonMouseOn(WPARAM btnState, int x, int y);

		//What state is the button in
		int GetState() { return mState; }
		void SetState(int bs) { mState = bs; }

		int GetScriptType() { return mScriptType; }
		void SetScriptType(int st) { mScriptType = st; }

		//string GetScript() { return mScript; }
		//void SetScript(string script) { mScript = script; }

		void SetScript(Script* s) { mScript = s; }
		Script* GetScript() { return mScript; }

		bool WasLastStateUP() { return mUP && (mState == EButtonState::eNormal || mState == EButtonState::eHighlighted); }
		void ResetUPState()   { mUP = false; }

		float GetXPos() { return mXPos; }
		float GetYPos() { return mYPos; }
		int GetWidth() { return mWidth; }
		int GetHeight() { return mHeight; }

		void MoveButtonX(float amount);
		void MoveButtonY(float amount);

		void SetWidth(int w);
		void SetHeight(int h);

		void CloneMe();

		void SetNormalImage(string name);
		void SetPushedImage(string name);
		void SetHighlightedImage(string name);
		void SetDisabledImage(string name);

		string GetNormalImageName();
		string GetPushedImageName();
		string GetHighlightedImageName();
		string GetDisabledImageName();

		void SetText(string text);
		string GetText();

		Text GetTextComponent() { return mButtonText; }

		//void Draw10(bool useTransparent, Camera* camera, TextureShader* textureShader, TransparentShader* transparentShader);
		void Draw(bool useTransparent, Camera* camera);

	private:
		D3D*              mD3D;
		int			      mState;

		int				  mScriptType;
		//string            mScript;
		Script*           mScript;

		D3D* m_d3d;
		string m_windowName;
		GameDirectory2D* m_gd;
		EngineOptions* m_eo;
		HWND m_hwnd;

		bool              mUP;//Was last state up
		bool              mDisableHighlight;

		float             mXPos;
		float             mYPos;
		int               mWidth;
		int               mHeight;

		Text             mButtonText;
		XMMATRIX         mTextTransform;

		Image*    mNormalButtonImage;
		Image*    mPushedButtonImage;
		Image*    mHighlightedButtonImage;
		Image*    mDisabledButtonImage;

		string mButtonNormalName;
		string mButtonPushedName;
		string mButtonHighlightedName;
		string mButtonDisabledName;

		string mButtonNormalBackgroundColor;
		string mButtonPushedBackgroundColor;
		string mButtonHighlightedBackgroundColor;
		string mButtonDisabledBackgroundColor;
	};
}
//-----------------------------------------------------------------------------------------
#endif//__BUTTON_H