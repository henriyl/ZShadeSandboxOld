#include "Menu.h"
#include "Convert.h"
//#include "Commands.h"
using ZShadeSandboxEnvironment::Menu;
//--------------------------------------------------------------------------------------------------------------------
Menu::Menu() : m_background(NULL)
{
}
//--------------------------------------------------------------------------------------------------------------------
Menu::Menu(const Menu& o)
{

}
//--------------------------------------------------------------------------------------------------------------------
Menu::~Menu()
{
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo)
{
	vector<ZShadeSandboxGraphics::Button*> new_buttons;
	vector<ZShadeSandboxGraphics::Text*> new_texts;

	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* pb = m_menu_buttons[i];

		if (pb != NULL)
		{
			ZShadeSandboxGraphics::Button* b = new ZShadeSandboxGraphics::Button(d3d, gd, eo, ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(pb->GetText()), false,
				pb->GetXPos(), pb->GetYPos(),
				pb->GetWidth(), pb->GetHeight(),
				pb->GetNormalImageName(),
				pb->GetPushedImageName(),
				pb->GetHighlightedImageName(),
				pb->GetDisabledImageName(),
				"white.png",
				"blue.png",
				"yellow.png",
				"black.png");

			new_buttons.push_back( b );
		}
	}

	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* pt = m_menu_text[i];
		
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

	if (m_menu_buttons.size() > 0)
	{
		m_menu_buttons.clear();

		for (int i = 0; i < new_buttons.size(); i++)
		{
			m_menu_buttons.push_back( new_buttons[i] );
		}
	}

	if (m_menu_text.size() > 0)
	{
		m_menu_text.clear();

		for (int i = 0; i < new_texts.size(); i++)
		{
			m_menu_text.push_back( new_texts[i] );
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::RemoveButton(ZShadeSandboxGraphics::Button* b)
{
	int value = 0;
	bool kill = false;
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* pb = m_menu_buttons[i];

		if (pb != NULL)
		{
			if (pb == b)
			{
				value = i;
				kill = true;
				break;
			}
		}
	}

	if (kill)
	{
		m_menu_buttons.erase(m_menu_buttons.begin() + value);
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::RemoveText(ZShadeSandboxGraphics::Text* t)
{
	int value = 0;
	bool kill = false;
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* pt = m_menu_text[i];

		if (pt->TextName() == t->TextName())
		{
			value = i;
			kill = true;
			break;
		}
	}

	if (kill)
	{
		m_menu_text.erase(m_menu_text.begin() + value);
	}
}
//--------------------------------------------------------------------------------------------------------------------
bool Menu::MenuButtonSelected(ZShadeSandboxGraphics::Button*& button, int x, int y)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* b = m_menu_buttons[i];

		if (b != NULL)
		{
			if (x >= b->GetXPos() && x <= b->GetXPos() + b->GetWidth())
			{
				if (y >= b->GetYPos() && y <= b->GetYPos() + b->GetHeight())
				{
					button = b;
					return true;
				}
			}
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
bool Menu::MenuTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y)
{
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = m_menu_text[i];

		if (x >= t->GetXPos() && x <= t->GetXPos() + t->GetWidth())
		{
			if (y >= t->GetYPos() && y <= t->GetYPos() + t->GetHeight())
			{
				text = t;
				return true;
			}
		}
	}

	return false;
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::AddBackground(ZShadeSandboxGraphics::Image* image)
{
	m_background = image;
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::AddMenuButton(ZShadeSandboxGraphics::Button* button)
{
	m_menu_buttons.push_back( button );
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::AddMenuText(ZShadeSandboxGraphics::Text* text)
{
	m_menu_text.push_back( text );
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::UpdateButtonMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		for (int i = 0; i < m_menu_buttons.size(); i++)
		{
			ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

			if (button != NULL)
			{
				button->UpdateButtonMouseUp(btnState, x, y);

				if (button->WasLastStateUP())
				{
					button->ResetUPState();
					//Need to have the buttons do something
					//Should make this happen from a .lua script

					//Need to assign a .lua script to each button
					//Create something like a LuaButton that is a struct with
					//  a button and a .lua file
					switch (button->GetScriptType())
					{
						case ZShadeSandboxGraphics::EScriptType::eStart:
						case ZShadeSandboxGraphics::EScriptType::eContinue:
						case ZShadeSandboxGraphics::EScriptType::eOptions:
						case ZShadeSandboxGraphics::EScriptType::eExit:
						{
							if (button->GetScript() != NULL)
								button->GetScript()->Main();
						}
						break;
						default: break;
					}
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::UpdateButtonMouseDown(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		for (int i = 0; i < m_menu_buttons.size(); i++)
		{
			ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

			if (button != NULL)
			{
				button->UpdateButtonMouseDown(btnState, x, y);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::UpdateButtonMouseOn(WPARAM btnState, int x, int y)
{
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

		if (button != NULL)
		{
			button->UpdateButtonMouseOn(btnState, x, y);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------
void Menu::UpdateButtonMouseMove(WPARAM btnState, int x, int y)
{
	// Move the selected item

}
//--------------------------------------------------------------------------------------------------------------------
/*bool Menu::Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
{
	//Render the background sprite
	if (m_background != NULL) m_background->Draw(true, textureShader, transparentShader, camera);

	//Render all the menu buttons
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		Button* button = m_menu_buttons[i];

		if (button != NULL)
		{
			button->Draw10(true, camera, textureShader, transparentShader);
		}
	}

	//Render all the menu text
	//for (int i = 0; i < m_menu_text.size(); i++)
	//{
		//Text* text = m_menu_text[i];
		//text.Render11();
	//}

	return true;
}*/
//--------------------------------------------------------------------------------------------------------------------
bool Menu::Render(Camera* camera)
{
	//Render the background sprite
	if (m_background != NULL)
		m_background->Draw(false, camera);

	//Render all the menu buttons
	for (int i = 0; i < m_menu_buttons.size(); i++)
	{
		ZShadeSandboxGraphics::Button* button = m_menu_buttons[i];

		if (button != NULL)
		{
			button->Draw(true, camera);
		}
	}

	//Render all the menu text
	for (int i = 0; i < m_menu_text.size(); i++)
	{
		ZShadeSandboxGraphics::Text* text = m_menu_text[i];
		
		if (text != NULL)
		{
			text->Render11();
		}
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------------------