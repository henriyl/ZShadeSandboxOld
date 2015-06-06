#include "Button.h"
using ZShadeSandboxGraphics::Button;
//-----------------------------------------------------------------------------------------
Button::Button(D3D* d3d, GameDirectory2D* gd, EngineOptions* eo, char* text, bool disableHighlight,
	float x, float y, int width, int height,
	string normalImageName, string pushedImageName, string highlightedImageName, string disabledImageName,
	string pNormalBackgroundColor, string pPushedBackgroundColor, string pHighlightedBackgroundColor, string pDisabledBackgroundColor)
:   mXPos(x), mYPos(y), mWidth(width), mHeight(height), mDisableHighlight(disableHighlight), mD3D(d3d), mUP(false)
{
	mScript = NULL;
	mScriptType = EScriptType::eNone;

	m_gd = gd;
	m_eo = eo;
	m_d3d = d3d;

	mXPos = x;
	mYPos = y;

	//Button starts out in normal state
	mState = EButtonState::eNormal;

	mButtonNormalBackgroundColor = pNormalBackgroundColor;
	mButtonPushedBackgroundColor = pPushedBackgroundColor;
	mButtonHighlightedBackgroundColor = pHighlightedBackgroundColor;
	mButtonDisabledBackgroundColor = pDisabledBackgroundColor;

	//Make image the background if exists
	//Make the text the center of the button

	mButtonNormalName = normalImageName;
	mButtonPushedName = pushedImageName;
	mButtonHighlightedName = highlightedImageName;
	mButtonDisabledName = disabledImageName;

	mNormalButtonImage = new Image( normalImageName, gd->m_sprites_path, x, y, width, height, d3d, gd->m_required_textures, mButtonNormalBackgroundColor );
	mPushedButtonImage = new Image( pushedImageName, gd->m_sprites_path, x, y, width, height, d3d, gd->m_required_textures, mButtonPushedBackgroundColor );
	mHighlightedButtonImage = new Image( highlightedImageName, gd->m_sprites_path, x, y, width, height, d3d, gd->m_required_textures, mButtonHighlightedBackgroundColor );
	mDisabledButtonImage = new Image( disabledImageName, gd->m_sprites_path, x, y, width, height, d3d, gd->m_required_textures, mButtonDisabledBackgroundColor );

	//mButtonText = new Text( text, windowName, gd, x + width / 4 - 4, y + height / 4 - 2, d3d, text_color, p_color, hwnd, eo );

	mTextTransform = XMMatrixTranslation(x + width / 4 - 4, y + height / 4 - 2, 0);

	mButtonText.SetD3D(d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = text;
	if (mButtonText.TextName() == "NONE")
		mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
Button::Button(D3D* d3d, GameDirectory2D* gd, EngineOptions* eo, char* text, bool disableHighlight,
	float x, float y, int width, int height,
	string base_button_path,
	string normalImageName, string pushedImageName, string highlightedImageName, string disabledImageName,
	string pNormalBackgroundColor, string pPushedBackgroundColor, string pHighlightedBackgroundColor, string pDisabledBackgroundColor)
:   mXPos(x), mYPos(y), mWidth(width), mHeight(height), mDisableHighlight(disableHighlight), mD3D(d3d), mUP(false)
{
	mScript = NULL;
	mScriptType = EScriptType::eNone;

	m_gd = gd;
	m_eo = eo;
	m_d3d = d3d;

	//Button starts out in normal state
	mState = EButtonState::eNormal;

	mButtonNormalBackgroundColor = pNormalBackgroundColor;
	mButtonPushedBackgroundColor = pPushedBackgroundColor;
	mButtonHighlightedBackgroundColor = pHighlightedBackgroundColor;
	mButtonDisabledBackgroundColor = pDisabledBackgroundColor;

	//Make image the background if exists
	//Make the text the center of the button

	mButtonNormalName = normalImageName;
	mButtonPushedName = pushedImageName;
	mButtonHighlightedName = highlightedImageName;
	mButtonDisabledName = disabledImageName;

	mNormalButtonImage = new Image( normalImageName, base_button_path, x, y, width, height, d3d, gd->m_required_textures, mButtonNormalBackgroundColor );
	mPushedButtonImage = new Image( pushedImageName, base_button_path, x, y, width, height, d3d, gd->m_required_textures, mButtonPushedBackgroundColor );
	mHighlightedButtonImage = new Image( highlightedImageName, base_button_path, x, y, width, height, d3d, gd->m_required_textures, mButtonHighlightedBackgroundColor );
	mDisabledButtonImage = new Image( disabledImageName, base_button_path, x, y, width, height, d3d, gd->m_required_textures, mButtonDisabledBackgroundColor );

	//mButtonText = new Text( text, windowName, gd, x + width/4 - 4, y + height/4 - 2, d3d, text_color, p_color, hwnd, eo );

	mTextTransform = XMMatrixTranslation(x + width / 4 - 4, y + height / 4 - 2, 0);

	mButtonText.SetD3D(d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = text;
	if (mButtonText.TextName() == "NONE")
		mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
Button::~Button()
{
	delete mNormalButtonImage;
	delete mPushedButtonImage;
	delete mHighlightedButtonImage;
	delete mDisabledButtonImage;
}
//-----------------------------------------------------------------------------------------
string Button::GetNormalImageName()
{
	if (mNormalButtonImage != NULL)
		return mNormalButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetPushedImageName()
{
	if (mPushedButtonImage != NULL)
		return mPushedButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetHighlightedImageName()
{
	if (mHighlightedButtonImage != NULL)
		return mHighlightedButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
string Button::GetDisabledImageName()
{
	if (mDisabledButtonImage != NULL)
		return mDisabledButtonImage->GetName();
	else
		return "NONE";
}
//-----------------------------------------------------------------------------------------
void Button::SetText(string text)
{
	mButtonText.TextName() = BetterString(text);
}
//-----------------------------------------------------------------------------------------
string Button::GetText()
{
	return mButtonText.TextName().toString();
}
//-----------------------------------------------------------------------------------------
void Button::SetNormalImage(string name)
{
	mNormalButtonImage = new Image( name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d );
}
//-----------------------------------------------------------------------------------------
void Button::SetPushedImage(string name)
{
	mPushedButtonImage = new Image( name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d );
}
//-----------------------------------------------------------------------------------------
void Button::SetHighlightedImage(string name)
{
	mHighlightedButtonImage = new Image( name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d );
}
//-----------------------------------------------------------------------------------------
void Button::SetDisabledImage(string name)
{
	mDisabledButtonImage = new Image( name, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d );
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseUp(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->GetXPos();
	r.right = r.left + mNormalButtonImage->GetWidth();// + 50;
	r.top = mNormalButtonImage->GetYPos();
	r.bottom = r.top + mNormalButtonImage->GetHeight();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;
	
	if (!PtInRect(&r, pt))
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseDown(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->GetXPos();
	r.right = r.left + mNormalButtonImage->GetWidth();// + 50;
	r.top = mNormalButtonImage->GetYPos();
	r.bottom = r.top + mNormalButtonImage->GetHeight();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;

	if (PtInRect(&r, pt))
	{
		mUP = true;
		mState = EButtonState::ePushed;
	}
	else
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::UpdateButtonMouseOn(WPARAM btnState, int x, int y)
{
	if (mState == EButtonState::eDisabled) return;

	if (strcmp(mButtonText.TextName(), "NONE") == 0)
		mButtonText.TextName() = "";

	RECT r;
	r.left = mNormalButtonImage->GetXPos();
	r.right = r.left + mNormalButtonImage->GetWidth();// + 50;
	r.top = mNormalButtonImage->GetYPos();
	r.bottom = r.top + mNormalButtonImage->GetHeight();// + 20;

	POINT pt;
	pt.x = x;
	pt.y = y;

	if (PtInRect(&r, pt))
	{
		mState = EButtonState::eHighlighted;
	}
	else
	{
		mState = EButtonState::eNormal;
	}
}
//-----------------------------------------------------------------------------------------
void Button::MoveButtonX(float amount)
{
	mNormalButtonImage->MoveImageX( amount );
	mPushedButtonImage->MoveImageX( amount );
	mHighlightedButtonImage->MoveImageX( amount );
	mDisabledButtonImage->MoveImageX( amount );

	mTextTransform = XMMatrixTranslation(amount + mWidth / 4 - 4, mYPos, 0);

	mButtonText.TextTransform() = mTextTransform;

	mXPos = mNormalButtonImage->GetXPos();
	mWidth = mNormalButtonImage->GetWidth();
}
//-----------------------------------------------------------------------------------------
void Button::MoveButtonY(float amount)
{
	mNormalButtonImage->MoveImageY( amount );
	mPushedButtonImage->MoveImageY( amount );
	mHighlightedButtonImage->MoveImageY( amount );
	mDisabledButtonImage->MoveImageY( amount );

	mTextTransform = XMMatrixTranslation(mXPos, amount + mHeight / 4 - 2, 0);

	mButtonText.TextTransform() = mTextTransform;

	mYPos = mNormalButtonImage->GetYPos();
	mHeight = mNormalButtonImage->GetHeight();
}
//-----------------------------------------------------------------------------------------
void Button::SetWidth(int w)
{
	mWidth = w;
}
//-----------------------------------------------------------------------------------------
void Button::SetHeight(int h)
{
	mHeight = h;
}
//-----------------------------------------------------------------------------------------
void Button::CloneMe()
{
	mNormalButtonImage = new Image( mButtonNormalName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d, m_gd->m_required_textures, mButtonNormalBackgroundColor );
	mPushedButtonImage = new Image( mButtonPushedName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d, m_gd->m_required_textures, mButtonPushedBackgroundColor );
	mHighlightedButtonImage = new Image( mButtonHighlightedName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d, m_gd->m_required_textures, mButtonHighlightedBackgroundColor );
	mDisabledButtonImage = new Image( mButtonDisabledName, m_gd->m_sprites_path, mXPos, mYPos, mWidth, mHeight, m_d3d, m_gd->m_required_textures, mButtonDisabledBackgroundColor );
	//mButtonText = new Text( mButtonText->GetText(), m_windowName, m_gd, mXPos + mWidth/4 - 4, mYPos + mHeight/4 - 2, m_d3d, mButtonText->GetColor11(), mButtonText->GetColor10(), m_hwnd, m_eo );

	mTextTransform = XMMatrixTranslation(mXPos + mWidth / 4 - 4, mYPos + mHeight / 4 - 2, 0);

	mButtonText.SetD3D(m_d3d);
	mButtonText.Initialize();
	mButtonText.ChangeFont(L"Times New Roman", 12.0f, FontStyle::BoldItalic, true);
	mButtonText.TextName() = "";
	mButtonText.TextTransform() = mTextTransform;
	mButtonText.TextColor() = XMFLOAT4(1, 1, 1, 1);
}
//-----------------------------------------------------------------------------------------
/*void Button::Draw10(bool useTransparent, Camera* camera, TextureShader* textureShader, TransparentShader* transparentShader)
{
	if (mState == BNORMAL)
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(useTransparent, textureShader, transparentShader, camera);
	}

	if (!mDisableHighlight)
	{
		if (mState == BHIGHLIGHTED)
		{
			if (mHighlightedButtonImage)
				mHighlightedButtonImage->Draw(useTransparent, textureShader, transparentShader, camera);
		}
	}
	else
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(useTransparent, textureShader, transparentShader, camera);
	}

	if (mState == BPUSHED)
	{
		if (mPushedButtonImage)
			mPushedButtonImage->Draw(useTransparent, textureShader, transparentShader, camera);
	}

	if (mState == BDISABLED)
	{
		if (mDisabledButtonImage)
			mDisabledButtonImage->Draw(useTransparent, textureShader, transparentShader, camera);
	}

	//mButtonText->DrawFont10(d3d, useTransparent, textureShader, transparentShader, worldMatrix, orthoMatrix, viewMatrix);
	//mButtonText.Render11(mTextTransform, XMFLOAT4(1, 1, 1, 1));
}*/
//-----------------------------------------------------------------------------------------
void Button::Draw(bool useTransparent, Camera* camera)
{
	if (mState == EButtonState::eNormal)
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(useTransparent, camera);
	}

	if (!mDisableHighlight)
	{
		if (mState == EButtonState::eHighlighted)
		{
			if (mHighlightedButtonImage)
				mHighlightedButtonImage->Draw(useTransparent, camera);
		}
	}
	else
	{
		if (mNormalButtonImage)
			mNormalButtonImage->Draw(useTransparent, camera);
	}

	if (mState == EButtonState::ePushed)
	{
		if (mPushedButtonImage)
			mPushedButtonImage->Draw(useTransparent, camera);
	}

	if (mState == EButtonState::eDisabled)
	{
		if (mDisabledButtonImage)
			mDisabledButtonImage->Draw(useTransparent, camera);
	}

	mButtonText.Render11();
}
//-----------------------------------------------------------------------------------------