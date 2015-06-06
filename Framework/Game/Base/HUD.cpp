#include "HUD.h"
#include "Convert.h"
//--------------------------------------------------------------------------------------------------------------------
HUD::HUD(D3D* d3d)
:   m_D3DSystem(d3d)
{
}
//--------------------------------------------------------------------------------------------------------------------
HUD::HUD(const HUD& h)
{
}
//--------------------------------------------------------------------------------------------------------------------
HUD::~HUD()
{
}
//--------------------------------------------------------------------------------------------------------------------
void HUD::ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo)
{
	vector<ZShadeSandboxGraphics::Image*> new_images;
	vector<ZShadeSandboxGraphics::Text*> new_texts;

	for (int i = 0; i < m_images.size(); i++)
	{
		ZShadeSandboxGraphics::Image* im = m_images[i];

		if (im != NULL)
		{
			ZShadeSandboxGraphics::Image* nim = new ZShadeSandboxGraphics::Image(im->GetName(), gd->m_hud_path, im->GetXPos(), im->GetYPos(), im->GetWidth(), im->GetHeight(), d3d);
			new_images.push_back( nim );
		}
	}

	for (int i = 0; i < m_texts.size(); i++)
	{
		ZShadeSandboxGraphics::Text* pt = m_texts[i];

		if (pt != NULL)
		{
			ZShadeSandboxGraphics::Text* npt = new ZShadeSandboxGraphics::Text();
			npt->SetD3D(d3d);
			npt->Initialize();
			npt->TextName() = pt->TextName();
			if (pt->HasShadow()) npt->CreateShadow();
			npt->ChangeFont(L"Times New Roman", pt->FontSize(), FontStyle::BoldItalic, true);
			npt->TextTransform() = XMMatrixTranslation(pt->GetXPos(), pt->GetYPos(), 0);
			XMFLOAT4 tColor = pt->TextColor();
			npt->TextColor() = XMFLOAT4(tColor.x, tColor.y, tColor.z, 1);
			new_texts.push_back(npt);
		}
	}

	if (m_images.size() > 0)
	{
		m_images.clear();

		for (int i = 0; i < new_images.size(); i++)
		{
			m_images.push_back( new_images[i] );
		}
	}

	if (m_texts.size() > 0)
	{
		m_texts.clear();

		for (int i = 0; i < new_texts.size(); i++)
		{
			m_texts.push_back( new_texts[i] );
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void HUD::RemoveImage(ZShadeSandboxGraphics::Image* im)
{
	int value = 0;
	bool kill = false;
	for (int i = 0; i < m_images.size(); i++)
	{
		ZShadeSandboxGraphics::Image* p_im = m_images[i];

		if (p_im != NULL)
		{
			if (p_im == im)
			{
				value = i;
				kill = true;
				break;
			}
		}
	}

	if (kill)
	{
		m_images.erase(m_images.begin() + value);
	}
}
//--------------------------------------------------------------------------------------------------------------------
void HUD::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	int value = 0;
	bool kill = false;
	for (int i = 0; i < m_texts.size(); i++)
	{
		ZShadeSandboxGraphics::Text* pt = m_texts[i];

		if (pt != NULL)
		{
			if (pt == t)
			{
				value = i;
				kill = true;
				break;
			}
		}
	}

	if (kill)
	{
		m_texts.erase(m_texts.begin() + value);
	}
}
//--------------------------------------------------------------------------------------------------------------------
bool HUD::HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y)
{
	for (int i = 0; i < m_images.size(); i++)
	{
		ZShadeSandboxGraphics::Image* p_im = m_images[i];

		if (p_im != NULL)
		{
			if (x >= p_im->GetXPos() && x <= p_im->GetXPos() + p_im->GetWidth())
			{
				if (y >= p_im->GetYPos() && y <= p_im->GetYPos() + p_im->GetHeight())
				{
					im = p_im;
					return true;
				}
			}
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
bool HUD::HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_texts.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_texts[i];

		if (t != NULL)
		{
			if (x >= t->GetXPos() && x <= t->GetXPos() + t->GetWidth())
			{
				if (y >= t->GetYPos() && y <= t->GetYPos() + t->GetHeight())
				{
					text = t;
					return true;
				}
			}
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
void HUD::AddImage(ZShadeSandboxGraphics::Image* im)
{
	m_images.push_back( im );
}
//--------------------------------------------------------------------------------------------------------------------
void HUD::AddText(ZShadeSandboxGraphics::Text* t)
{
	m_texts.push_back( t );
}
//--------------------------------------------------------------------------------------------------------------------
/*void HUD::Draw10(bool useTransparent, TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
{
	if (!m_images.empty())
	{
		for (int i = 0; i < m_images.size(); i++)
		{
			m_images[i]->Draw(useTransparent, textureShader, transparentShader, camera);
		}
	}

	if (!m_texts.empty())
	{
		for (int i = 0; i < m_texts.size(); i++)
		{
			//m_texts[i]->DrawFont10(d3d, useTransparent, textureShader, transparentShader, worldMatrix, orthoMatrix, viewMatrix);
		}
	}
}*/
//--------------------------------------------------------------------------------------------------------------------
void HUD::Draw(bool useTransparent, Camera* camera)
{
	if (!m_images.empty())
	{
		for (int i = 0; i < m_images.size(); i++)
		{
			m_images[i]->Draw(useTransparent, camera);
		}
	}

	if (!m_texts.empty())
	{
		for (int i = 0; i < m_texts.size(); i++)
		{
			m_texts[i]->Render11();
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------