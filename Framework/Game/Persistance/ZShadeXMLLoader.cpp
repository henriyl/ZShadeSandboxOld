#include "ZShadeXMLLoader.h"
#include "Convert.h"
#include "BetterString.h"
#include "ShaderMaterial.h"
#include "MaterialManager.h"
#include "Scripting.h"
//===============================================================================================================================
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMaterialXML(string filename, D3D* d3d)
{
	string fname;
	fname.append(filename);
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile( fname.c_str() );
	XMLNode* rootnode = doc.FirstChild();
	XMLElement* rootElement = doc.FirstChildElement();
	
	// Get the name of the material
	XMLElement* materialNameElement = rootElement->FirstChildElement("Name");
	const char* material_name_str = materialNameElement->GetText();
	
	// Can the material use a shadow map
	XMLElement* enableShadowMapElement = rootElement->FirstChildElement("EnableShadowMap");
	const char* enable_shadowmap_str = enableShadowMapElement->GetText();
	
	// Can the material use SSAO
	XMLElement* enableSSAOElement = rootElement->FirstChildElement("EnableSSAO");
	const char* enable_ssao_str = enableSSAOElement->GetText();
	
	// Is the material transparent
	XMLElement* transparencyElement = rootElement->FirstChildElement("EnableTransparency");
	const char* transparency_str = transparencyElement->GetText();
	
	// Can light shine on the material
	XMLElement* enableLightingElement = rootElement->FirstChildElement("EnableLighting");
	const char* enable_lighting_str = enableLightingElement->GetText();
	
	// Get the brightness of the material
	XMLElement* detailBrightnessElement = rootElement->FirstChildElement("DetailBrightness");
	const char* detail_brightness_str = detailBrightnessElement->GetText();
	
	// Get the alpha coverage values for transparency
	XMLElement* alphaToCoverageElement = rootElement->FirstChildElement("AlphaToCoverage");
	const char* alpha_to_coverage_str = alphaToCoverageElement->GetText();
	
	// Get the materials ambient color
	XMLElement* ambientColorElement = rootElement->FirstChildElement("AmbientColor");
	const char* ambient_color_str = ambientColorElement->GetText();
	
	// Get the materials diffuse color
	XMLElement* diffuseColorElement = rootElement->FirstChildElement("DiffuseColor");
	const char* diffuse_color_str = diffuseColorElement->GetText();
	
	// Get the materials specular color
	XMLElement* specularColorElement = rootElement->FirstChildElement("SpecularColor");
	const char* specular_color_str = specularColorElement->GetText();
	
	// Get the specular exponent value for specular lighting
	XMLElement* specularPowerElement = rootElement->FirstChildElement("SpecularPower");
	const char* specular_power_str = specularPowerElement->GetText();
	
	// Get the specular intensity value for specular lighting
	XMLElement* specularIntensityElement = rootElement->FirstChildElement("SpecularIntensity");
	const char* specular_intensity_str = specularIntensityElement->GetText();
	
	// Get the emissive, reflectivity and transmittivity values for radiance of light
	XMLElement* emissiveElement = rootElement->FirstChildElement("Emissive");
	const char* emissive_str = emissiveElement->GetText();
	XMLElement* reflectivityElement = rootElement->FirstChildElement("Reflectivity");
	const char* reflectivity_str = reflectivityElement->GetText();
	XMLElement* transmissivityElement = rootElement->FirstChildElement("Transmissivity");
	const char* transmissivity_str = transmissivityElement->GetText();
	
	// Load the diffuse array texture names
	XMLElement* images_tag = rootElement->FirstChildElement("DiffuseArray");
	
	vector<string> names;
	
	//Now loop through all the tiles in the section
	for (XMLElement* image_e = images_tag->FirstChildElement("Texture"); image_e != NULL; image_e = image_e->NextSiblingElement("Texture"))
	{
		string eName = image_e->Value();
		string eIDAtt = image_e->Attribute("id");
		
		// Get the name of the texture
		XMLElement* texNameElement = image_e->FirstChildElement("TexName");
		const char* tex_name_str = texNameElement->GetText();
		
		names.push_back(string(tex_name_str));
	}
	
	// Get the diffuse texture of the material
	XMLElement* diffuseTextureElement = rootElement->FirstChildElement("DiffuseTexture");
	const char* diffuse_texture_str = diffuseTextureElement->GetText();
	
	// Get the detail texture of the material
	XMLElement* detailTextureElement = rootElement->FirstChildElement("DetailTexture");
	const char* detail_texture_str = detailTextureElement->GetText();
	
	// Get the normal map (bump) texture of the material
	XMLElement* normalTextureElement = rootElement->FirstChildElement("NormalTexture");
	const char* normal_texture_str = normalTextureElement->GetText();
	
	// Get the blend map texture of the material
	XMLElement* blendTextureElement = rootElement->FirstChildElement("BlendTexture");
	const char* blend_texture_str = blendTextureElement->GetText();
	
	// Get the alpha map texture of the material
	XMLElement* alphaTextureElement = rootElement->FirstChildElement("AlphaTexture");
	const char* alpha_texture_str = alphaTextureElement->GetText();
	
	string name(material_name_str);
	bool enableShadowMap;
	bool enableSSAO;
	bool enableTransparency;
	bool enableLighting;
	float detailBrightness;
	XMFLOAT3 alphaToCoverage;
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT4 specularColor;
	float specularPower;
	float specularIntensity;
	float emissive;
	float reflectivity;
	float transmissivity;
	string diffuseTextureName(diffuse_texture_str);
	string detailTextureName(detail_texture_str);
	string normalTextureName(normal_texture_str);
	string blendTextureName(blend_texture_str);
	string alphaTextureName(alpha_texture_str);
	
	(BetterString(enable_shadowmap_str) == "true") ? enableShadowMap = true : enableShadowMap = false;
	(BetterString(enable_ssao_str) == "true") ? enableSSAO = true : enableSSAO = false;
	(BetterString(transparency_str) == "true") ? enableTransparency = true : enableTransparency = false;
	(BetterString(enable_lighting_str) == "true") ? enableLighting = true : enableLighting = false;
	
	detailBrightness = atof(detail_brightness_str);
	
	BetterString alphaStr(alpha_to_coverage_str);
	vector<string> alpha_values = alphaStr.split(' ');
	alphaToCoverage.x = atof(alpha_values[0].c_str());
	alphaToCoverage.y = atof(alpha_values[1].c_str());
	alphaToCoverage.z = atof(alpha_values[2].c_str());
	
	BetterString ambientStr(ambient_color_str);
	vector<string> ambient_values = ambientStr.split(' ');
	ambientColor.x = atof(ambient_values[0].c_str());
	ambientColor.y = atof(ambient_values[1].c_str());
	ambientColor.z = atof(ambient_values[2].c_str());
	ambientColor.w = atof(ambient_values[3].c_str());
	
	BetterString diffuseStr(diffuse_color_str);
	vector<string> diffuse_values = diffuseStr.split(' ');
	diffuseColor.x = atof(diffuse_values[0].c_str());
	diffuseColor.y = atof(diffuse_values[1].c_str());
	diffuseColor.z = atof(diffuse_values[2].c_str());
	diffuseColor.w = atof(diffuse_values[3].c_str());
	
	BetterString specularStr(specular_color_str);
	vector<string> specular_values = specularStr.split(' ');
	specularColor.x = atof(specular_values[0].c_str());
	specularColor.y = atof(specular_values[1].c_str());
	specularColor.z = atof(specular_values[2].c_str());
	specularColor.w = atof(specular_values[3].c_str());
	
	specularPower = atof(specular_power_str);
	specularIntensity = atof(specular_intensity_str);
	emissive = atof(emissive_str);
	reflectivity = atof(reflectivity_str);
	transmissivity = atof(transmissivity_str);
	
	//
	// Load the material into the material manager
	//
	
	ZShadeSandboxLighting::ShaderMaterial* material = new ZShadeSandboxLighting::ShaderMaterial(d3d, name);
	
	material->EnableShadowMap() = enableShadowMap;
	material->EnableSSAOMap() = enableSSAO;
	material->EnableTransparency() = enableTransparency;
	material->EnableLighting() = enableLighting;
	material->DetailBrightness() = detailBrightness;
	material->AlphaToCoverageValue() = alphaToCoverage;
	material->AmbientColor() = ambientColor;
	material->DiffuseColor() = diffuseColor;
	material->SpecularColor() = specularColor;
	
	bool hasDiffuseArrayTexture;
	(BetterString(names[0]) == "NONE") ? hasDiffuseArrayTexture = false : hasDiffuseArrayTexture = true;
	if (hasDiffuseArrayTexture)
		material->CreateTexture2DArray(names);
	
	material->SpecularPower() = specularPower;
	material->SpecularIntensity() = specularIntensity;
	material->Emissivity() = emissive;
	material->Reflectivity() = reflectivity;
	material->Transmissivity() = transmissivity;
	
	// Need to add the material path for 3D
	//string game_material_path = 
	
	bool hasDiffuseTexture;
	(BetterString(diffuseTextureName) == "NONE") ? hasDiffuseTexture = false : hasDiffuseTexture = true;
	if (hasDiffuseTexture)
		material->AddDiffuseTexture("", diffuseTextureName);
	
	bool hasDetailTexture;
	(BetterString(detailTextureName) == "NONE") ? hasDetailTexture = false : hasDetailTexture = true;
	if (hasDetailTexture)
		material->AddDetailMapTexture("", detailTextureName);
	
	bool hasNormalMapTexture;
	(BetterString(normalTextureName) == "NONE") ? hasNormalMapTexture = false : hasNormalMapTexture = true;
	if (hasNormalMapTexture)
		material->AddNormalMapTexture("", normalTextureName);
	
	bool hasBlendMapTexture;
	(BetterString(blendTextureName) == "NONE") ? hasBlendMapTexture = false : hasBlendMapTexture = true;
	if (hasBlendMapTexture)
		material->AddBlendMapTexture("", blendTextureName);
	
	bool hasAlphaMapTexture;
	(BetterString(alphaTextureName) == "NONE") ? hasAlphaMapTexture = false : hasAlphaMapTexture = true;
	if (hasAlphaMapTexture)
		material->AddAlphaMapTexture("", alphaTextureName);
	
	MaterialManager::Instance()->Add(material);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMenuXML(string menufilename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HWND hwnd, MenuSystem*& ms)
{
	string fname;

	fname.append(menufilename);

	tinyxml2::XMLDocument doc;

	doc.LoadFile( fname.c_str() );

	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	string menu_name;
	int menu_width;
	int menu_height;
	ZShadeSandboxEnvironment::EMenuType::Type menuType;
	string background_name;

	//Sprite *spr;
	//Now loop through all the sprites in the map
	//for (XMLElement* e = rootElement->FirstChildElement("Menu"); e != NULL; e = e->NextSiblingElement("Menu"))
	{
		//string eName = e->Value();
		//string eIDAtt = e->Attribute("id");

		//Load the menu name
		XMLElement* menuNameElement = rootElement->FirstChildElement("menu_name");
		const char* menu_name_str = menuNameElement->GetText();
		menu_name = string(menu_name_str);
		
		//Load the menu width and height
		XMLElement* menuSizeElement = rootElement->FirstChildElement("menu_size");
		const char* menu_size_str = menuSizeElement->GetText();
		vector<string> split_str;// = CGlobal::split(string(menu_size_str), 'x');
		BetterString split_b_str(menu_size_str);
		split_str = split_b_str.split('x');
		menu_width = atoi(split_str[0].c_str());
		menu_height = atoi(split_str[1].c_str());

		//Load the menu type
		XMLElement* menuTypeElement = rootElement->FirstChildElement("menu_type");
		const char* menu_type_str = menuTypeElement->GetText();
		if (strcmp(menu_type_str, "MainMenu") == 0)
			menuType = ZShadeSandboxEnvironment::EMenuType::eMainMenu;
		else if (strcmp(menu_type_str, "InGameMenu") == 0)
			menuType = ZShadeSandboxEnvironment::EMenuType::eInGameMenu;

		//Load the menu background name
		XMLElement* menuBackgroundElement = rootElement->FirstChildElement("menu_background");
		const char* menu_background_str = menuBackgroundElement->GetText();
		background_name = string(menu_background_str);

		ZShadeSandboxEnvironment::Menu* m = new ZShadeSandboxEnvironment::Menu();
		m->SetMenuName(menu_name);
		m->SetMenuWidth(menu_width);
		m->SetMenuHeight(menu_height);
		m->SetMenuType(menuType);
		
		if (background_name != "")
		{
			ZShadeSandboxGraphics::Image* im = new ZShadeSandboxGraphics::Image(background_name, gd->m_sprites_path, 0, 0, menu_width, menu_height, d3d);
			m->AddBackground( im );
		}

		ms->AddMenu(m);
		//ms->SetMenuToRender(menu_name);

		//Load the buttons
		XMLElement* buttons_tag = rootElement->FirstChildElement("Buttons");

		float button_x;
		float button_y;
		int button_width;
		int button_height;
		string button_text;
		string normal_image;
		string pushed_image;
		string highlighted_image;
		string disabled_image;
		string action_script;
		int st = ZShadeSandboxGraphics::EScriptType::eNone;

		//Now loop through all the tiles in the section
		for (XMLElement* button_e = buttons_tag->FirstChildElement("Button"); button_e != NULL; button_e = button_e->NextSiblingElement("Button"))
		{
			string eName = button_e->Value();
			string eIDAtt = button_e->Attribute("id");

			//Load the x pos of the button
			XMLElement* buttonXElement = button_e->FirstChildElement("button_x");
			const char* button_x_str = buttonXElement->GetText();
			button_x = atof( button_x_str );

			//Load the y pos of the button
			XMLElement* buttonYElement = button_e->FirstChildElement("button_y");
			const char* button_y_str = buttonYElement->GetText();
			button_y = atof( button_y_str );

			//Load the width of the button
			XMLElement* buttonWidthElement = button_e->FirstChildElement("button_width");
			const char* button_width_str = buttonWidthElement->GetText();
			button_width = atoi( button_width_str );

			//Load the height of the button
			XMLElement* buttonHeightElement = button_e->FirstChildElement("button_height");
			const char* button_height_str = buttonHeightElement->GetText();
			button_height = atoi( button_height_str );

			//Load the text of the button
			XMLElement* buttonTextElement = button_e->FirstChildElement("button_text");
			const char* button_text_str = buttonTextElement->GetText();
			if (strcmp(button_text_str, "NONE") != 0)
				button_text = string(button_text_str);
			else
				button_text = "NONE";

			//Load the normal image of the button
			XMLElement* normalImageElement = button_e->FirstChildElement("normal_image");
			const char* normal_image_str = normalImageElement->GetText();
			if (strcmp(normal_image_str, "NONE") != 0)
				normal_image = string(normal_image_str);
			else
				normal_image = "NONE";

			//Load the pushed image of the button
			XMLElement* pushedImageElement = button_e->FirstChildElement("pushed_image");
			const char* pushed_image_str = pushedImageElement->GetText();
			if (strcmp(pushed_image_str, "NONE") != 0)
				pushed_image = string(pushed_image_str);
			else
				pushed_image = "NONE";

			//Load the highlighted image of the button
			XMLElement* highlightedImageElement = button_e->FirstChildElement("highlighted_image");
			const char* highlighted_image_str = highlightedImageElement->GetText();
			if (strcmp(highlighted_image_str, "NONE") != 0)
				highlighted_image = string(highlighted_image_str);
			else
				highlighted_image = "NONE";

			//Load the disabled image of the button
			XMLElement* disabledImageElement = button_e->FirstChildElement("disabled_image");
			const char* disabled_image_str = disabledImageElement->GetText();
			if (strcmp(disabled_image_str, "NONE") != 0)
				disabled_image = string(disabled_image_str);
			else
				disabled_image = "NONE";

			//Load the script type for the button
			XMLElement* scriptTypeElement = button_e->FirstChildElement("script_type");
			const char* script_type_str = scriptTypeElement->GetText();
			if (strcmp(script_type_str, "start") == 0) st = ZShadeSandboxGraphics::EScriptType::eStart;
			if (strcmp(script_type_str, "resume") == 0) st = ZShadeSandboxGraphics::EScriptType::eResume;
			if (strcmp(script_type_str, "continue") == 0) st = ZShadeSandboxGraphics::EScriptType::eContinue;
			if (strcmp(script_type_str, "options") == 0) st = ZShadeSandboxGraphics::EScriptType::eOptions;
			if (strcmp(script_type_str, "exit") == 0) st = ZShadeSandboxGraphics::EScriptType::eExit;
			if (strcmp(script_type_str, "none") == 0) st = ZShadeSandboxGraphics::EScriptType::eNone;

			////Load the action script for the button
			//XMLElement* actionScriptElement = button_e->FirstChildElement("action_script");
			//const char* action_script_str = actionScriptElement->GetText();
			//if (strcmp(action_script_str, "NONE") != 0)
			//{
			//	action_script = string(action_script_str);
			//	//Add the .lua
			//	action_script.append(".lua");
			//}
			//else
			//	action_script = "NONE";

			ZShadeSandboxGraphics::Button* b = new ZShadeSandboxGraphics::Button(d3d, gd, eo, ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(button_text), false,
				button_x, button_y,
				button_width, button_height,
				gd->m_menu_sprites_path,
				normal_image,
				pushed_image,
				highlighted_image,
				disabled_image,
				"white.png",
				"blue.png",
				"yellow.png",
				"black.png");

			//b->SetScript(action_script);
			b->SetScriptType(st);

			switch (st)
			{
				case ZShadeSandboxGraphics::EScriptType::eStart: b->SetScript(Scripting::GetScript("start")); break;
				case ZShadeSandboxGraphics::EScriptType::eResume: b->SetScript(Scripting::GetScript("resume")); break;
				case ZShadeSandboxGraphics::EScriptType::eContinue: b->SetScript(Scripting::GetScript("continue")); break;
				case ZShadeSandboxGraphics::EScriptType::eOptions: b->SetScript(Scripting::GetScript("options")); break;
				case ZShadeSandboxGraphics::EScriptType::eExit: b->SetScript(Scripting::GetScript("exit")); break;
			}

			//ms->AddMenuButton(menu_name, b);
			m->AddMenuButton(b);
		}

		float text_x;
		float text_y;
		int text_width;
		int text_height;
		string t_text;
		int font_size;
		float font_color_r;
		float font_color_g;
		float font_color_b;
		int has_shadow;
		string optional_background_name;

		//Load the texts
		XMLElement* texts_tag = rootElement->FirstChildElement("Texts");
		//Now loop through all the tiles in the section
		for (XMLElement* text_e = texts_tag->FirstChildElement("Text"); text_e != NULL; text_e = text_e->NextSiblingElement("Text"))
		{
			string eName = text_e->Value();
			string eIDAtt = text_e->Attribute("id");

			//Load the x pos of the text
			XMLElement* textXElement = text_e->FirstChildElement("text_x");
			const char* text_x_str = textXElement->GetText();
			text_x = atof( text_x_str );

			//Load the y pos of the text
			XMLElement* textYElement = text_e->FirstChildElement("text_y");
			const char* text_y_str = textYElement->GetText();
			text_y = atof( text_y_str );

			////Load the width of the text
			//XMLElement* textWidthElement = text_e->FirstChildElement("text_width");
			//const char* text_width_str = textWidthElement->GetText();
			//text_width = atoi( text_width_str );

			////Load the height of the text
			//XMLElement* textHeightElement = text_e->FirstChildElement("text_height");
			//const char* text_height_str = textWidthElement->GetText();
			//text_height = atoi( text_height_str );

			//Load the text of the text
			XMLElement* textElement = text_e->FirstChildElement("text");
			const char* text_str = textElement->GetText();
			t_text = string(text_str);

			//Load the font size of the text
			XMLElement* textFontElement = text_e->FirstChildElement("text_font");
			const char* text_font_str = textFontElement->GetText();
			font_size = atoi(string(text_font_str).c_str());

			//Load the font color of the text
			XMLElement* textColorElement = text_e->FirstChildElement("text_color");
			const char* text_color_str = textColorElement->GetText();
			vector<string> split_font_str;// = CGlobal::split(string(text_color_str), ' ');
			BetterString split_b_font_str(text_color_str);
			split_font_str = split_b_font_str.split(' ');
			font_color_r = atof(split_font_str[0].c_str());
			font_color_g = atof(split_font_str[1].c_str());
			font_color_b = atof(split_font_str[2].c_str());

			//Does the text have a shadow
			XMLElement* shadowElement = text_e->FirstChildElement("shadow");
			const char* shadow_str = shadowElement->GetText();
			has_shadow = atoi(string(shadow_str).c_str());

			//Load the optional background of the text
			XMLElement* textOptionalBackgroundElement = text_e->FirstChildElement("optional_background");
			const char* text_optional_background_str = textOptionalBackgroundElement->GetText();
			if (text_optional_background_str != 0)
				optional_background_name = string(text_optional_background_str);
			else
				optional_background_name = "";

			XMVECTORF32 color = { font_color_r, font_color_g, font_color_b, 1.0f };

			ZShadeSandboxGraphics::Text* t = new ZShadeSandboxGraphics::Text();
			t->SetD3D(d3d);
			t->Initialize();
			t->TextName() = BetterString(t_text);
			if (has_shadow == 1)
				t->CreateShadow();
			t->ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
			t->TextTransform() = XMMatrixTranslation(text_x, text_y, 0);
			t->TextColor() = XMFLOAT4(font_color_r, font_color_g, font_color_b, 1);

			//Text* t = new Text(ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(t_text), "Render", gd, text_x, text_y, d3d, color, z_rgb, hwnd, eo, optional_background_name);
			//t->UpdateFontSize(font_size);

			//ms->AddMenuText( menu_name, t );
			m->AddMenuText(t);
		}
	}

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, Camera* cam, D3D* d3d, TopdownMap*& map)
{
	string fname;

	fname.append(spriteMapfilename);

	tinyxml2::XMLDocument doc;

	doc.LoadFile( fname.c_str() );

	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	//Do not know if I need map_name
	//XMLElement* mapNameElement = rootnode->FirstChildElement("map_name");
	//const char* map_name_str = mapNameElement->GetText();

	float x;
	float y;
	int w;
	int h;
	float hbox_x;
	float hbox_y;
	int hbox_w;
	int hbox_h;
	int vision;
	int depth;
	int hard;
	int nohit;
	int hp;
	int hp_max;
	bool is_static;
	int mana;
	int gold;
	int strength;
	float speed;
	int defense;
	int exp;
	int level;
	int touch;
	int background;
	int entity;
	int invisible;
	int teleport;
	int teleport_map_x;
	int teleport_map_y;
	int default_seq;
	int anim_speed;
	bool is_player;

	AISprite *spr;
	//Now loop through all the sprites in the map
	for (XMLElement* e = rootElement->FirstChildElement("Sprite"); e != NULL; e = e->NextSiblingElement("Sprite"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");

		//Get whether or not the sprite is a player
		XMLElement* playerElement = e->FirstChildElement("player");
		const char* player_str = playerElement->GetText();
		if (strcmp(player_str, "true") == 0)
			is_player = true;
		else if (strcmp(player_str, "false") == 0)
			is_player = false;

		//Get the x attribute for the sprite
		XMLElement* xElement = e->FirstChildElement("x");
		const char* x_str = xElement->GetText();
		x = atof( x_str );

		//Get the y attribute for the sprite
		XMLElement* yElement = e->FirstChildElement("y");
		const char* y_str = yElement->GetText();
		y = atof( y_str );

		//Get the width attribute for the sprite
		XMLElement* widthElement = e->FirstChildElement("width");
		const char* width_str = widthElement->GetText();
		w = atoi( width_str );

		//Get the height attribute for the sprite
		XMLElement* heightElement = e->FirstChildElement("height");
		const char* height_str = heightElement->GetText();
		h = atoi( height_str );

		//Get the hardbox x attribute for the sprite
		XMLElement* hbox_xElement = e->FirstChildElement("hbox_x");
		const char* hbox_x_str = hbox_xElement->GetText();
		hbox_x = atof( hbox_x_str );

		//Get the hardbox y attribute for the sprite
		XMLElement* hbox_yElement = e->FirstChildElement("hbox_y");
		const char* hbox_y_str = hbox_yElement->GetText();
		hbox_y = atof( hbox_y_str );

		//Get the hardbox width attribute for the sprite
		XMLElement* hbox_widthElement = e->FirstChildElement("hbox_width");
		const char* hbox_width_str = hbox_widthElement->GetText();
		hbox_w = atoi( hbox_width_str );

		//Get the hardbox height attribute for the sprite
		XMLElement* hbox_heightElement = e->FirstChildElement("hbox_height");
		const char* hbox_height_str = hbox_heightElement->GetText();
		hbox_h = atoi( hbox_height_str );

		//Get the baseimage attribute for the sprite
		XMLElement* baseImageElement = e->FirstChildElement("baseimage");
		const char* base_image_str = baseImageElement->GetText();
		
		//Get the vision attribute for the sprite
		XMLElement* visionElement = e->FirstChildElement("vision");
		const char* vision_str = visionElement->GetText();
		vision = atoi( vision_str );

		//Get the depth attribute for the sprite
		XMLElement* depthElement = e->FirstChildElement("depth");
		const char* depth_str = depthElement->GetText();
		depth = atoi( depth_str );

		//Get the hard attribute for the sprite
		XMLElement* hardElement = e->FirstChildElement("hard");
		const char* hard_str = hardElement->GetText();
		hard = atoi( hard_str );

		//Get the nohit attribute for the sprite
		XMLElement* nohitElement = e->FirstChildElement("nohit");
		const char* nohit_str = nohitElement->GetText();
		nohit = atoi( nohit_str );
		
		//Get the animation profile name attribute for the sprite
		XMLElement* animationElement = e->FirstChildElement("animation");
		const char* animation_str = animationElement->GetText();

		//Get the default animation sequence attribute for the sprite
		XMLElement* default_seqElement = e->FirstChildElement("default_seq");
		const char* default_seq_str = default_seqElement->GetText();
		default_seq = atoi( default_seq_str );

		//Get the anim speed attribute for the sprite
		XMLElement* anim_speedElement = e->FirstChildElement("anim_speed");
		const char* anim_speed_str = anim_speedElement->GetText();
		anim_speed = atoi( anim_speed_str );

		//Get the hp attribute for the sprite
		XMLElement* hpElement = e->FirstChildElement("hp");
		const char* hp_str = hpElement->GetText();
		hp = atoi( hp_str );

		//Get the hp attribute for the sprite
		XMLElement* hpMaxElement = e->FirstChildElement("hp_max");
		const char* hp_max_str = hpMaxElement->GetText();
		hp_max = atoi( hp_max_str );

		//Get the mana attribute for the sprite
		XMLElement* manaElement = e->FirstChildElement("mana");
		const char* mana_str = manaElement->GetText();
		mana = atoi( mana_str );

		//Get the gold attribute for the sprite
		XMLElement* goldElement = e->FirstChildElement("gold");
		const char* gold_str = goldElement->GetText();
		gold = atoi( gold_str );

		//Get the strength attribute for the sprite
		XMLElement* strengthElement = e->FirstChildElement("strength");
		const char* strength_str = strengthElement->GetText();
		strength = atoi( strength_str );

		//Get the speed attribute for the sprite
		XMLElement* speedElement = e->FirstChildElement("speed");
		const char* speed_str = speedElement->GetText();
		speed = atof( speed_str );

		//Get the defense attribute for the sprite
		XMLElement* defenseElement = e->FirstChildElement("defense");
		const char* defense_str = defenseElement->GetText();
		defense = atoi( defense_str );

		//Get the experience attribute for the sprite
		XMLElement* experienceElement = e->FirstChildElement("experience");
		const char* experience_str = experienceElement->GetText();
		exp = atoi( experience_str );

		//Get the level attribute for the sprite
		XMLElement* levelElement = e->FirstChildElement("level");
		const char* level_str = levelElement->GetText();
		level = atoi( level_str );

		//Get the script attribute for the sprite
		XMLElement* scriptElement = e->FirstChildElement("script");
		const char* script_str = scriptElement->GetText();

		//Get the touch attribute for the sprite
		XMLElement* touchElement = e->FirstChildElement("touch");
		const char* touch_str = touchElement->GetText();
		touch = atoi( touch_str );

		//Get the background attribute for the sprite
		XMLElement* backgroundElement = e->FirstChildElement("background");
		const char* background_str = backgroundElement->GetText();
		background = atoi( background_str );

		//Get the entity attribute for the sprite
		XMLElement* entityElement = e->FirstChildElement("entity");
		const char* entity_str = entityElement->GetText();
		entity = atoi( entity_str );

		//Get the invisible attribute for the sprite
		XMLElement* invisibleElement = e->FirstChildElement("invisible");
		const char* invisible_str = invisibleElement->GetText();
		invisible = atoi( invisible_str );

		//Get the invisible attribute for the sprite
		XMLElement* isStaticElement = e->FirstChildElement("static");
		const char* is_static_str = isStaticElement->GetText();
		is_static = atoi( is_static_str );

		//Get the name attribute for the sprite
		XMLElement* nameElement = e->FirstChildElement("name");
		const char* name_str = nameElement->GetText();

		//Get the collide msg attribute for the sprite
		XMLElement* collideMsgElement = e->FirstChildElement("collide_msg");
		const char* collideMsg_str = collideMsgElement->GetText();

		//Get the teleport attribute for the sprite
		XMLElement* teleportElement = e->FirstChildElement("teleport");
		const char* teleport_str = teleportElement->GetText();
		teleport = atoi( teleport_str );

		//Get the teleport map name attribute for the sprite
		XMLElement* teleportMapElement = e->FirstChildElement("teleport_map");
		const char* teleportMap_str = teleportMapElement->GetText();

		//Get the teleport map x attribute for the sprite
		XMLElement* teleportMapXElement = e->FirstChildElement("teleport_map_x");
		const char* teleportMapX_str = teleportMapXElement->GetText();
		teleport_map_x = atoi( teleportMapX_str );

		//Get the teleport map y attribute for the sprite
		XMLElement* teleportMapYElement = e->FirstChildElement("teleport_map_y");
		const char* teleportMapY_str = teleportMapYElement->GetText();
		teleport_map_y = atoi( teleportMapY_str );

		string path;
		if (strcmp(base_image_str, "player_cover.png") == 0)
		{
			path = gd->m_required_textures;
		}
		else
		{
			path = gd->m_sprites_path;
		}

		PhysicsType pt;
		if (is_static)
			pt = STATIC;
		else
			pt = DYNAMIC;

		spr = new AISprite(d3d);
		spr->Initialize(path, string(base_image_str), XMFLOAT3(x, y, 0), w, h, pt);
		
		//string sc = string(script_str);
		//sc.append(".lua");
		spr->ScriptName() = string(script_str);
		
		spr->PlatformerSprite() = false; // This is a sprite on a TopDown map
		spr->ID() = atoi(eIDAtt.c_str());
		spr->Vision() = vision;
		spr->Depth() = depth;
		spr->IsHard() = hard;
		spr->IsNoHit() = nohit;
		spr->AnimationProfileName() = animation_str;
		spr->DefaultSequence() = default_seq;
		spr->AnimationSpeed() = anim_speed;
		spr->HP() = hp;
		spr->Mana() = mana;
		spr->Gold() = gold;
		spr->Strength() = strength;
		spr->Speed() = speed;
		spr->Defense() = defense;
		spr->Experience() = exp;
		spr->Level() = level;
		spr->IsTouch() = touch;
		spr->IsBackgroundSprite() = background;
		spr->IsEntitySprite() = entity;
		spr->IsInvisibleSprite() = invisible;
		spr->CanTeleport() = teleport;
		spr->TeleportMapName() = teleportMap_str;
		spr->TeleportX() = teleport_map_x;
		spr->TeleportY() = teleport_map_y;
		spr->Name() = name_str;
		spr->HPMax() = hp_max;
		spr->IsStatic() = is_static;
		spr->CollideMsg() = string(collideMsg_str);
		
		//Set the sprites hardbox
		spr->SetHardbox(
			hbox_x, hbox_y, hbox_w, hbox_h//x, y + h / 2, w, h / 2
		);
		
		path = gd->m_required_textures;
		
		//Create the new hard box
		spr->CreateHardBoxSpr(path);

		if (is_player)
		{
			//Add the sprite to the map
			map->AddPlayer(spr);
		}
		else
		{
			//Add the sprite to the map
			map->AddSprite(spr);
		}

		//Add the animation
		if (strcmp(animation_str, "NONE") != 0)
			map->AddSpriteAnimations(string(animation_str), gd->GAME_FOLDER, gd->m_sprites_path, spr->ID());
	}

	return true;
}
//================================================================================================================
bool ZShadeXMLLoader::LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, Camera* cam, D3D* d3d, PlatformerMap*& map)
{
	string fname;

	fname.append(spriteMapfilename);

	tinyxml2::XMLDocument doc;

	doc.LoadFile( fname.c_str() );

	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	float x;
	float y;
	int w;
	int h;
	float hbox_x;
	float hbox_y;
	int hbox_w;
	int hbox_h;
	//int vision;
	//int depth;
	int hard;
	int nohit;
	int hp;
	int hp_max;
	bool is_static;
	//int mana;
	//int gold;
	int coin;
	int strength;
	float speed;
	//int defense;
	//int exp;
	//int level;
	int touch;
	int background;
	int entity;
	int invisible;
	int teleport;
	int teleport_map_x;
	int teleport_map_y;
	int default_seq;
	int anim_speed;
	bool is_player;

	AISprite *spr;
	//Now loop through all the sprites in the map
	for (XMLElement* e = rootElement->FirstChildElement("Sprite"); e != NULL; e = e->NextSiblingElement("Sprite"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");

		//Get whether or not the sprite is a player
		XMLElement* playerElement = e->FirstChildElement("player");
		const char* player_str = playerElement->GetText();
		if (strcmp(player_str, "true") == 0)
			is_player = true;
		else if (strcmp(player_str, "false") == 0)
			is_player = false;

		//Get the x attribute for the sprite
		XMLElement* xElement = e->FirstChildElement("x");
		const char* x_str = xElement->GetText();
		x = atof( x_str );

		//Get the y attribute for the sprite
		XMLElement* yElement = e->FirstChildElement("y");
		const char* y_str = yElement->GetText();
		y = atof( y_str );

		//Get the width attribute for the sprite
		XMLElement* widthElement = e->FirstChildElement("width");
		const char* width_str = widthElement->GetText();
		w = atoi( width_str );

		//Get the height attribute for the sprite
		XMLElement* heightElement = e->FirstChildElement("height");
		const char* height_str = heightElement->GetText();
		h = atoi( height_str );

		//Get the hardbox x attribute for the sprite
		XMLElement* hbox_xElement = e->FirstChildElement("hbox_x");
		const char* hbox_x_str = hbox_xElement->GetText();
		hbox_x = atof( hbox_x_str );

		//Get the hardbox y attribute for the sprite
		XMLElement* hbox_yElement = e->FirstChildElement("hbox_y");
		const char* hbox_y_str = hbox_yElement->GetText();
		hbox_y = atof( hbox_y_str );

		//Get the hardbox width attribute for the sprite
		XMLElement* hbox_widthElement = e->FirstChildElement("hbox_width");
		const char* hbox_width_str = hbox_widthElement->GetText();
		hbox_w = atoi( hbox_width_str );

		//Get the hardbox height attribute for the sprite
		XMLElement* hbox_heightElement = e->FirstChildElement("hbox_height");
		const char* hbox_height_str = hbox_heightElement->GetText();
		hbox_h = atoi( hbox_height_str );

		//Get the baseimage attribute for the sprite
		XMLElement* baseImageElement = e->FirstChildElement("baseimage");
		const char* base_image_str = baseImageElement->GetText();

		//Get the vision attribute for the sprite
		//XMLElement* visionElement = e->FirstChildElement("vision");
		//const char* vision_str = visionElement->GetText();
		//vision = atoi( vision_str );

		//Get the depth attribute for the sprite
		//XMLElement* depthElement = e->FirstChildElement("depth");
		//const char* depth_str = depthElement->GetText();
		//depth = atoi( depth_str );

		//Get the hard attribute for the sprite
		XMLElement* hardElement = e->FirstChildElement("hard");
		const char* hard_str = hardElement->GetText();
		hard = atoi( hard_str );

		//Get the nohit attribute for the sprite
		XMLElement* nohitElement = e->FirstChildElement("nohit");
		const char* nohit_str = nohitElement->GetText();
		nohit = atoi( nohit_str );
		
		//Get the animation profile name attribute for the sprite
		XMLElement* animationElement = e->FirstChildElement("animation");
		const char* animation_str = animationElement->GetText();

		//Get the default animation sequence attribute for the sprite
		XMLElement* default_seqElement = e->FirstChildElement("default_seq");
		const char* default_seq_str = default_seqElement->GetText();
		default_seq = atoi( default_seq_str );

		//Get the anim speed attribute for the sprite
		XMLElement* anim_speedElement = e->FirstChildElement("anim_speed");
		const char* anim_speed_str = anim_speedElement->GetText();
		anim_speed = atoi( anim_speed_str );

		//Get the hp attribute for the sprite
		XMLElement* hpElement = e->FirstChildElement("hp");
		const char* hp_str = hpElement->GetText();
		hp = atoi( hp_str );

		//Get the hp attribute for the sprite
		XMLElement* hpMaxElement = e->FirstChildElement("hp_max");
		const char* hp_max_str = hpMaxElement->GetText();
		hp_max = atoi( hp_max_str );
		
		//Get the coin attribute for the sprite
		XMLElement* coinElement = e->FirstChildElement("coin");
		const char* coin_str = coinElement->GetText();
		coin = atoi( coin_str );

		//Get the strength attribute for the sprite
		XMLElement* strengthElement = e->FirstChildElement("strength");
		const char* strength_str = strengthElement->GetText();
		strength = atoi( strength_str );

		//Get the speed attribute for the sprite
		XMLElement* speedElement = e->FirstChildElement("speed");
		const char* speed_str = speedElement->GetText();
		speed = atof( speed_str );
		
		//Get the mana attribute for the sprite
		//XMLElement* manaElement = e->FirstChildElement("mana");
		//const char* mana_str = manaElement->GetText();
		//mana = atoi( mana_str );
		
		//Get the defense attribute for the sprite
		//XMLElement* defenseElement = e->FirstChildElement("defense");
		//const char* defense_str = defenseElement->GetText();
		//defense = atoi( defense_str );
		
		//Get the experience attribute for the sprite
		//XMLElement* experienceElement = e->FirstChildElement("experience");
		//const char* experience_str = experienceElement->GetText();
		//exp = atoi( experience_str );
		
		//Get the level attribute for the sprite
		//XMLElement* levelElement = e->FirstChildElement("level");
		//const char* level_str = levelElement->GetText();
		//level = atoi( level_str );

		//Get the script attribute for the sprite
		XMLElement* scriptElement = e->FirstChildElement("script");
		const char* script_str = scriptElement->GetText();

		//Get the touch attribute for the sprite
		XMLElement* touchElement = e->FirstChildElement("touch");
		const char* touch_str = touchElement->GetText();
		touch = atoi( touch_str );

		//Get the background attribute for the sprite
		XMLElement* backgroundElement = e->FirstChildElement("background");
		const char* background_str = backgroundElement->GetText();
		background = atoi( background_str );

		//Get the entity attribute for the sprite
		XMLElement* entityElement = e->FirstChildElement("entity");
		const char* entity_str = entityElement->GetText();
		entity = atoi( entity_str );

		//Get the invisible attribute for the sprite
		XMLElement* invisibleElement = e->FirstChildElement("invisible");
		const char* invisible_str = invisibleElement->GetText();
		invisible = atoi( invisible_str );

		//Get the invisible attribute for the sprite
		XMLElement* isStaticElement = e->FirstChildElement("static");
		const char* is_static_str = isStaticElement->GetText();
		is_static = atoi( is_static_str );

		//Get the name attribute for the sprite
		XMLElement* nameElement = e->FirstChildElement("name");
		const char* name_str = nameElement->GetText();

		//Get the collide msg attribute for the sprite
		XMLElement* collideMsgElement = e->FirstChildElement("collide_msg");
		const char* collideMsg_str = collideMsgElement->GetText();

		//Get the teleport attribute for the sprite
		XMLElement* teleportElement = e->FirstChildElement("teleport");
		const char* teleport_str = teleportElement->GetText();
		teleport = atoi( teleport_str );

		//Get the teleport map name attribute for the sprite
		XMLElement* teleportMapElement = e->FirstChildElement("teleport_map");
		const char* teleportMap_str = teleportMapElement->GetText();

		//Get the teleport map x attribute for the sprite
		XMLElement* teleportMapXElement = e->FirstChildElement("teleport_map_x");
		const char* teleportMapX_str = teleportMapXElement->GetText();
		teleport_map_x = atoi( teleportMapX_str );

		//Get the teleport map y attribute for the sprite
		XMLElement* teleportMapYElement = e->FirstChildElement("teleport_map_y");
		const char* teleportMapY_str = teleportMapYElement->GetText();
		teleport_map_y = atoi( teleportMapY_str );

		string path;
		if (strcmp(base_image_str, "player_cover.png") == 0)
		{
			path = gd->m_required_textures;
		}
		else
		{
			path = gd->m_sprites_path;
		}

		PhysicsType pt;
		if (is_static)
			pt = STATIC;
		else
			pt = DYNAMIC;

		spr = new AISprite(d3d);
		spr->Initialize(path, string(base_image_str), XMFLOAT3(x, y, 0), w, h, pt);
		
		//string sc = string(script_str);
		//sc.append(".lua");
		spr->ScriptName() = string(script_str);
		
		spr->PlatformerSprite() = true; // This is a sprite on a platformer map
		spr->ID() = atoi(eIDAtt.c_str());
		//spr->Vision() = vision;
		//spr->Depth() = depth;
		spr->IsHard() = hard;
		spr->IsNoHit() = nohit;
		spr->AnimationProfileName() = animation_str;
		spr->DefaultSequence() = default_seq;
		spr->AnimationSpeed() = anim_speed;
		spr->HP() = hp;
		//spr->Mana() = mana;
		spr->Coin() = coin;
		spr->Strength() = strength;
		spr->Speed() = speed;
		//spr->Defense() = defense;
		//spr->Experience() = exp;
		//spr->Level() = level;
		spr->IsTouch() = touch;
		spr->IsBackgroundSprite() = background;
		spr->IsEntitySprite() = entity;
		spr->IsInvisibleSprite() = invisible;
		spr->CanTeleport() = teleport;
		spr->TeleportMapName() = teleportMap_str;
		spr->TeleportX() = teleport_map_x;
		spr->TeleportY() = teleport_map_y;
		spr->Name() = name_str;
		spr->HPMax() = hp_max;
		spr->IsStatic() = is_static;
		spr->CollideMsg() = string(collideMsg_str);
		
		//spr->AddScript(string(script_str));

		//Set the sprites hardbox
		spr->SetHardbox(
			hbox_x, hbox_y, hbox_w, hbox_h//x, y + h / 2, w, h / 2
		);
		
		path = gd->m_required_textures;
		
		//Create the new hard box
		spr->CreateHardBoxSpr(path);

		if (is_player)
		{
			//Add the sprite to the map
			map->AddPlayer(spr);
		}
		else
		{
			//Add the sprite to the map
			map->AddSprite(spr);
		}

		//Add the animation
		if (strcmp(animation_str, "NONE") != 0)
			map->AddSpriteAnimations(string(animation_str), gd->GAME_FOLDER, gd->m_sprites_path, spr->ID());
	}

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadWorldXML(string worldname, SWorld*& sworld)
{
	if (sworld == NULL)
		sworld = new SWorld();

	string fname;

	fname.append(worldname);

	tinyxml2::XMLDocument doc;

	doc.LoadFile( fname.c_str() );
	
	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	XMLElement* nameElement = rootnode->FirstChildElement("name");
	const char* name_str = nameElement->GetText();
	
	sworld->sWorldName = name_str;

	XMLElement* map_tag = rootnode->FirstChildElement("Maps");

	//Now loop through all the sections in the map
	for (XMLElement* e = map_tag->FirstChildElement("Map"); e != NULL; e = e->NextSiblingElement("Map"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");

		//Get the vision attribute for the section
		XMLElement* mapNameElement = e->FirstChildElement("map_name");
		const char* map_name_str = mapNameElement->GetText();
		
		sworld->m_mapNames.push_back(map_name_str);
	}

	return true;
}
//==================================================================================================================================
bool ZShadeXMLLoader::LoadWorldXML(string worldname, SPlatformerWorld*& sworld)
{
	if (sworld == NULL)
		sworld = new SPlatformerWorld();

	string fname;

	fname.append(worldname);

	tinyxml2::XMLDocument doc;

	doc.LoadFile(fname.c_str());

	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	XMLElement* nameElement = rootnode->FirstChildElement("name");
	const char* name_str = nameElement->GetText();

	sworld->sWorldName = name_str;

	XMLElement* map_tag = rootnode->FirstChildElement("Maps");

	//Now loop through all the sections in the map
	for (XMLElement* e = map_tag->FirstChildElement("Map"); e != NULL; e = e->NextSiblingElement("Map"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");

		//Get the vision attribute for the section
		XMLElement* mapNameElement = e->FirstChildElement("map_name");
		const char* map_name_str = mapNameElement->GetText();

		sworld->m_mapNames.push_back(map_name_str);
	}

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadHUDXML(string filename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HWND hwnd, HUDSystem*& hs)
{
	string fname;

	fname.append(filename);

	tinyxml2::XMLDocument doc;

	doc.LoadFile( fname.c_str() );

	XMLNode* rootnode = doc.FirstChild();

	XMLElement* rootElement = doc.FirstChildElement();

	int width;
	int height;

	//Load the menu width and height
	XMLElement* hudSizeElement = rootElement->FirstChildElement("hud_size");
	const char* hud_size_str = hudSizeElement->GetText();
	BetterString hud_splite_str(hud_size_str);
	vector<string> split_str = hud_splite_str.split('x');
	width = atoi(split_str[0].c_str());
	height = atoi(split_str[1].c_str());

	hs->SetWidth(width);
	hs->SetHeight(height);

	//Load the images
	XMLElement* images_tag = rootElement->FirstChildElement("Images");

	float image_x;
	float image_y;
	int image_width;
	int image_height;
	string image_name;
	
	//Now loop through all the tiles in the section
	for (XMLElement* image_e = images_tag->FirstChildElement("Image"); image_e != NULL; image_e = image_e->NextSiblingElement("Image"))
	{
		string eName = image_e->Value();
		string eIDAtt = image_e->Attribute("id");

		//Load the x pos of the image
		XMLElement* imageXElement = image_e->FirstChildElement("image_x");
		const char* image_x_str = imageXElement->GetText();
		image_x = atof( image_x_str );

		//Load the y pos of the image
		XMLElement* imageYElement = image_e->FirstChildElement("image_y");
		const char* image_y_str = imageYElement->GetText();
		image_y = atof( image_y_str );

		//Load the width of the image
		XMLElement* imageWidthElement = image_e->FirstChildElement("image_width");
		const char* image_width_str = imageWidthElement->GetText();
		image_width = atoi( image_width_str );

		//Load the height of the image
		XMLElement* imageHeightElement = image_e->FirstChildElement("image_height");
		const char* image_height_str = imageHeightElement->GetText();
		image_height = atoi( image_height_str );

		//Load the name of the image
		XMLElement* imageNameElement = image_e->FirstChildElement("image_name");
		const char* image_name_str = imageNameElement->GetText();
		
		ZShadeSandboxGraphics::Image* im = new ZShadeSandboxGraphics::Image(string(image_name_str), gd->m_hud_path, image_x, image_y, image_width, image_height, d3d);

		hs->AddHUDImage(im);
	}

	float text_x;
	float text_y;
	int text_width;
	int text_height;
	string t_text;
	int font_size;
	float font_color_r;
	float font_color_g;
	float font_color_b;
	int has_shadow;
	string optional_background_name;

	//Load the texts
	XMLElement* texts_tag = rootElement->FirstChildElement("Texts");
	//Now loop through all the tiles in the section
	for (XMLElement* text_e = texts_tag->FirstChildElement("Text"); text_e != NULL; text_e = text_e->NextSiblingElement("Text"))
	{
		string eName = text_e->Value();
		string eIDAtt = text_e->Attribute("id");

		//Load the x pos of the text
		XMLElement* textXElement = text_e->FirstChildElement("text_x");
		const char* text_x_str = textXElement->GetText();
		text_x = atof( text_x_str );

		//Load the y pos of the text
		XMLElement* textYElement = text_e->FirstChildElement("text_y");
		const char* text_y_str = textYElement->GetText();
		text_y = atof( text_y_str );

		//Load the width of the text
		//XMLElement* textWidthElement = text_e->FirstChildElement("text_width");
		//const char* text_width_str = textWidthElement->GetText();
		//text_width = atoi( text_width_str );

		//Load the height of the text
		//XMLElement* textHeightElement = text_e->FirstChildElement("text_height");
		//const char* text_height_str = textWidthElement->GetText();
		//text_height = atoi( text_height_str );

		//Load the text of the text
		XMLElement* textElement = text_e->FirstChildElement("text");
		const char* text_str = textElement->GetText();
		t_text = string(text_str);

		//Load the font size of the text
		XMLElement* textFontElement = text_e->FirstChildElement("text_font");
		const char* text_font_str = textFontElement->GetText();
		font_size = atoi(string(text_font_str).c_str());

		//Load the font color of the text
		XMLElement* textColorElement = text_e->FirstChildElement("text_color");
		const char* text_color_str = textColorElement->GetText();
		vector<string> split_font_str;// = CGlobal::split(string(text_color_str), ' ');
		BetterString split_b_font_str(text_color_str);
		split_font_str = split_b_font_str.split(' ');
		font_color_r = atof(split_font_str[0].c_str());
		font_color_g = atof(split_font_str[1].c_str());
		font_color_b = atof(split_font_str[2].c_str());

		//Does the text have a shadow
		XMLElement* shadowElement = text_e->FirstChildElement("shadow");
		const char* shadow_str = textColorElement->GetText();
		has_shadow = atoi(string(shadow_str).c_str());

		//Load the optional background of the text
		XMLElement* textOptionalBackgroundElement = text_e->FirstChildElement("optional_background");
		const char* text_optional_background_str = textOptionalBackgroundElement->GetText();
		if (text_optional_background_str != 0)
			optional_background_name = string(text_optional_background_str);
		else
			optional_background_name = "";

		XMVECTORF32 color = { font_color_r, font_color_g, font_color_b, 1.0f };

		ZShadeSandboxGraphics::Text* t = new ZShadeSandboxGraphics::Text();
		t->SetD3D(d3d);
		t->Initialize();
		t->TextName() = BetterString(t_text);
		if (has_shadow == 1) t->CreateShadow();
		t->ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
		t->TextTransform() = XMMatrixTranslation(text_x, text_y, 0);
		t->TextColor() = XMFLOAT4(font_color_r, font_color_g, font_color_b, 1);
		
		//Text* t = new Text(ZShadeSandboxGlobal::Convert::ConvertStringToCharPointer(t_text), "Render", gd, text_x, text_y, d3d, color, z_rgb, hwnd, eo, optional_background_name);
		//t->UpdateFontSize(font_size);

		hs->AddHUDText(t);
	}

	return true;
}
//===============================================================================================================================
bool ZShadeXMLLoader::LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, Camera* cam, D3D* d3d, TopdownMap*& map, bool inEditor, HWND hwndPB)
{
	string fname;
	fname.append(filename);
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fname.c_str());

	int row_size;
	int col_size;
	bool fow_enabled;
	int fow_radius;
	bool fow_flashlight;

	XMLNode* rootnode = doc.FirstChild();
	XMLElement* rootElement = doc.FirstChildElement();

	XMLElement* mapNameElement = rootnode->FirstChildElement("map_name");
	const char* map_name_str = mapNameElement->GetText();
	mapname = map_name_str;

	XMLElement* xmlSpriteElement = rootnode->FirstChildElement("xml_sprite");
	const char* xml_sprite_str = xmlSpriteElement->GetText();

	XMLElement* mapTypeElement = rootnode->FirstChildElement("map_type");
	const char* map_type_str = mapTypeElement->GetText();

	XMLElement* rowSizeElement = rootnode->FirstChildElement("row_size");
	const char* row_size_str = rowSizeElement->GetText();

	XMLElement* colSizeElement = rootnode->FirstChildElement("col_size");
	const char* col_size_str = colSizeElement->GetText();

	row_size = atoi(row_size_str);
	col_size = atoi(col_size_str);

	d3d->GetEngineOptions()->SECTION_ROW_SIZE = row_size;
	d3d->GetEngineOptions()->SECTION_COL_SIZE = col_size;
	d3d->GetEngineOptions()->SetTileMax();

	XMLElement* fowElement = rootnode->FirstChildElement("Fow");
	fowElement->QueryBoolAttribute("enabled", &fow_enabled);
	fowElement->QueryIntAttribute("radius", &fow_radius);
	fowElement->QueryBoolAttribute("flashlight", &fow_flashlight);

	//d3d->GetEngineOptions()->m_fow = fow_enabled;
	//d3d->GetEngineOptions()->m_fow_radius = fow_radius;
	//d3d->GetEngineOptions()->m_fow_flashlight = fow_flashlight;

	MapLoadData mld;

	//Now loop through all the sections in the map
	for (XMLElement* e = rootElement->FirstChildElement("Section"); e != NULL; e = e->NextSiblingElement("Section"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");

		//Get the vision attribute for the section
		XMLElement* visionElement = e->FirstChildElement("Vision");
		const char* vision_str = visionElement->GetText();
		mld.sectionVision.push_back(atoi(vision_str));

		//Get the name of the music for the section
		XMLElement* musicElement = e->FirstChildElement("Music");
		const char* music_str = musicElement->GetText();
		mld.sectionMusicName.push_back(music_str);

		XMLElement* tile_tag = e->FirstChildElement("Tiles");

		SectionTileData std;

		//Now loop through all the tiles in the section
		for (XMLElement* tile_e = tile_tag->FirstChildElement("Tile"); tile_e != NULL; tile_e = tile_e->NextSiblingElement("Tile"))
		{
			string eName = tile_e->Value();
			string eIDAtt = tile_e->Attribute("id");
			string eImageNameAtt = tile_e->Attribute("image");

			//Get the animation profile name attribute for the sprite
			XMLElement* animationElement = tile_e->FirstChildElement("animation");
			const char* animation_str = animationElement->GetText();

			//Get the anim speed attribute for the sprite
			XMLElement* anim_speedElement = tile_e->FirstChildElement("anim_speed");
			const char* anim_speed_str = anim_speedElement->GetText();
			int anim_speed = atoi(anim_speed_str);

			//Get the hard attribute for the sprite
			XMLElement* hardElement = tile_e->FirstChildElement("hard");
			const char* hard_str = hardElement->GetText();
			int hard = atoi(hard_str);

			std.imageNames.push_back(eImageNameAtt);
			std.animProfileNames.push_back(string(animation_str));
			std.animSpeeds.push_back(anim_speed);
			std.hards.push_back(hard);
		}

		mld.sectionTileData.push_back(std);
	}

	Map2DType mt;
	if (BetterString(map_type_str) == "Regular")
		mt = Map2DType::REGULAR;
	else if (BetterString(map_type_str) == "Isometric")
		mt = Map2DType::ISOMETRIC;

	map = new TopdownMap(d3d, mt, cam);

	if (!map) return false;

	map->SetInEditor(inEditor);

	string path = gd->m_required_textures;
	//path.append("\\Textures");

	//Load the map sections with the map load data
	map->InitializeTiles(mld, gd, hwndPB);
	map->InitializePositionsAndPlayerAndRenderingSections(0, path);
	map->InitializeSectionPicker(path);

	map->SetMapSize(row_size);

	// Set the FOW data
	map->EnableFOW(fow_enabled);
	map->SetFOWRadius(fow_radius);
	map->EnableFOWFlashlight(fow_flashlight);

	BetterString str(gd->m_xml_sprites_path);
	str += "\\";
	str += xml_sprite_str;

	//Load the sprites onto the map
	LoadSpritesXML(str, gd, cam, d3d, map);

	return true;
}
//================================================================================================================
bool ZShadeXMLLoader::LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, Camera* cam, D3D* d3d, PlatformerMap*& map, bool inEditor, HWND hwndPB)
{
	string fname;
	fname.append(filename);
	tinyxml2::XMLDocument doc;
	doc.LoadFile(fname.c_str());
	
	PlatformerMapLoadData pmld;
	
	int map_width;
	int map_height;
	
	XMLNode* rootnode = doc.FirstChild();
	XMLElement* rootElement = doc.FirstChildElement();

	XMLElement* mapNameElement = rootnode->FirstChildElement("map_name");
	const char* map_name_str = mapNameElement->GetText();
	mapname = map_name_str;
	
	XMLElement* xmlSpriteElement = rootnode->FirstChildElement("xml_sprite");
	const char* xml_sprite_str = xmlSpriteElement->GetText();

	//C:\Users\Dustin\AppData\Roaming\Spazzy\XML\Sprites\BeginningSprites.xml
	
	XMLElement* xmlMusicElement = rootnode->FirstChildElement("music");
	const char* xml_music_str = xmlMusicElement->GetText();

	XMLElement* widthElement = rootnode->FirstChildElement("width");
	const char* width_str = widthElement->GetText();

	XMLElement* heightElement = rootnode->FirstChildElement("height");
	const char* height_str = heightElement->GetText();
	
	map_width = atoi(width_str);
	map_height = atoi(height_str);

	d3d->GetEngineOptions()->SECTION_ROW_SIZE = map_width;
	d3d->GetEngineOptions()->SECTION_COL_SIZE = map_height;
	d3d->GetEngineOptions()->SetTileMax();
	
	XMLElement* tiles_tag = rootnode->FirstChildElement("Tiles");

	pmld.exists.SetInitialValue(false);

	for (int y = 0; y < map_width; y++)
	{
		for (int x = 0; x < map_height; x++)
		{
			//pmld.exists[XMFLOAT2EXT(x, y)]
			pmld.exists.insert(XMFLOAT2EXT(x, y), 0);
		}
	}

	//Now loop through all the tiles in the map
	for (XMLElement* e = tiles_tag->FirstChildElement("Tile"); e != NULL; e = e->NextSiblingElement("Tile"))
	{
		string eName = e->Value();
		string eIDAtt = e->Attribute("id");
		string eImageNameAtt = e->Attribute("image");

		// Get the x pos attribute for the tile
		XMLElement* xElement = e->FirstChildElement("x");
		const char* x_str = xElement->GetText();
		int x = atoi(x_str);

		// Get the y pos attribute for the tile
		XMLElement* yElement = e->FirstChildElement("y");
		const char* y_str = yElement->GetText();
		int y = atoi(y_str);

		//Get the animation profile name attribute for the tile
		XMLElement* animationElement = e->FirstChildElement("animation");
		const char* animation_str = animationElement->GetText();

		//Get the anim speed attribute for the tile
		XMLElement* anim_speedElement = e->FirstChildElement("anim_speed");
		const char* anim_speed_str = anim_speedElement->GetText();
		int anim_speed = atoi(anim_speed_str);

		//Get the hard attribute for the tile
		XMLElement* hardElement = e->FirstChildElement("hard");
		const char* hard_str = hardElement->GetText();
		int hard = atoi(hard_str);

		pmld.imageNames.push_back(eImageNameAtt);
		pmld.animProfileNames.push_back(string(animation_str));
		pmld.animSpeeds.push_back(anim_speed);
		pmld.hards.push_back(hard);
		pmld.xs.push_back(PlatformerMap::PixelsToTiles(x));
		pmld.ys.push_back(PlatformerMap::PixelsToTiles(y));

		pmld.exists[XMFLOAT2EXT(PlatformerMap::PixelsToTiles(x), PlatformerMap::PixelsToTiles(y))] = 1;
		//pmld.exists.insert(XMFLOAT2EXT(PlatformerMap::PixelsToTiles(x), PlatformerMap::PixelsToTiles(y)), 1);
	}
	
	map = new PlatformerMap(d3d);
	if (!map) return false;
	
	map->MapName() = mapname;
	map->Width() = map_width;
	map->Height() = map_height;
	map->MusicName() = string(xml_music_str);

	map->InitializeTiles(pmld, gd, hwndPB);
	
	BetterString str(gd->m_xml_sprites_path);
	str += "\\";
	str += xml_sprite_str;

	//Load the sprites onto the map
	LoadSpritesXML(str.toString(), gd, cam, d3d, map);

	return true;
}
//===============================================================================================================================