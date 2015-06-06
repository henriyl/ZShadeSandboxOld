#include "ZShadeXMLSaver.h"
#include "tinyxml2.h"
#include "CGlobal.h"
#include "FogData.h"
#include <sstream>
#include <CommCtrl.h>
using namespace tinyxml2;
//===============================================================================================================================
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMaterialXML(string filename, ZShadeSandboxLighting::ShaderMaterial* material)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* materialSysNode = doc->InsertEndChild( doc->NewElement( "Material" ) );
	
	// Save the name of the material
	XMLElement* mat_name_elem = doc->NewElement( "Name" );
	mat_name_elem->InsertFirstChild( doc->NewText(material->Name().c_str()) );
	materialSysNode->InsertEndChild( mat_name_elem );
	
	// Save Enable flags of the material
	const char* enable_shadowmap_str = (material->EnableShadowMap() == true) ? "true" : "false";
	XMLElement* enable_shadowmap_elem = doc->NewElement( "EnableShadowMap" );
	enable_shadowmap_elem->InsertFirstChild( doc->NewText(enable_shadowmap_str) );
	materialSysNode->InsertEndChild( enable_shadowmap_elem );
	
	const char* enable_ssao_str = (material->EnableSSAOMap() == true) ? "true" : "false";
	XMLElement* enable_ssao_elem = doc->NewElement( "EnableSSAO" );
	enable_ssao_elem->InsertFirstChild( doc->NewText(enable_ssao_str) );
	materialSysNode->InsertEndChild( enable_ssao_elem );
	
	const char* enable_transparency_str = (material->EnableTransparency() == true) ? "true" : "false";
	XMLElement* enable_transparency_elem = doc->NewElement( "EnableTransparency" );
	enable_transparency_elem->InsertFirstChild( doc->NewText(enable_transparency_str) );
	materialSysNode->InsertEndChild( enable_transparency_elem );
	
	const char* enable_lighting_str = (material->EnableLighting() == true) ? "true" : "false";
	XMLElement* enable_lighting_elem = doc->NewElement( "EnableLighting" );
	enable_lighting_elem->InsertFirstChild( doc->NewText(enable_lighting_str) );
	materialSysNode->InsertEndChild( enable_lighting_elem );
	
	// Save the brightness of detail from the material
	stringstream db;
	db << material->DetailBrightness();
	const char* detail_brightness_str = db.str().c_str();
	XMLElement* detail_brightness_elem = doc->NewElement( "DetailBrightness" );
	detail_brightness_elem->InsertFirstChild( doc->NewText(detail_brightness_str) );
	materialSysNode->InsertEndChild( detail_brightness_elem );
	
	// Save the alpha coverage transparency from the material
	stringstream act_x;
	act_x << material->AlphaToCoverageValue().x;
	stringstream act_y;
	act_y << material->AlphaToCoverageValue().y;
	stringstream act_z;
	act_z << material->AlphaToCoverageValue().z;
	string act_str;
	act_str.append(act_x.str());
	act_str.append(" ");
	act_str.append(act_y.str());
	act_str.append(" ");
	act_str.append(act_z.str());
	XMLElement* alpha_coverage_elem = doc->NewElement( "AlphaToCoverage" );
	alpha_coverage_elem->InsertFirstChild( doc->NewText(act_str.c_str()) );
	materialSysNode->InsertEndChild( alpha_coverage_elem );
	
	// Save the materials ambient color
	stringstream amb_x;
	amb_x << material->AmbientColor().x;
	stringstream amb_y;
	amb_y << material->AmbientColor().y;
	stringstream amb_z;
	amb_z << material->AmbientColor().z;
	stringstream amb_w;
	amb_w << material->AmbientColor().w;
	string amb_str;
	amb_str.append(amb_x.str());
	amb_str.append(" ");
	amb_str.append(amb_y.str());
	amb_str.append(" ");
	amb_str.append(amb_z.str());
	amb_str.append(" ");
	amb_str.append(amb_w.str());
	XMLElement* ambient_elem = doc->NewElement( "AmbientColor" );
	ambient_elem->InsertFirstChild( doc->NewText(amb_str.c_str()) );
	materialSysNode->InsertEndChild( ambient_elem );
	
	// Save the materials diffuse color
	stringstream dif_x;
	dif_x << material->DiffuseColor().x;
	stringstream dif_y;
	dif_y << material->DiffuseColor().y;
	stringstream dif_z;
	dif_z << material->DiffuseColor().z;
	stringstream dif_w;
	dif_w << material->DiffuseColor().w;
	string dif_str;
	dif_str.append(dif_x.str());
	dif_str.append(" ");
	dif_str.append(dif_y.str());
	dif_str.append(" ");
	dif_str.append(dif_z.str());
	dif_str.append(" ");
	dif_str.append(dif_w.str());
	XMLElement* diffuse_elem = doc->NewElement( "DiffuseColor" );
	diffuse_elem->InsertFirstChild( doc->NewText(dif_str.c_str()) );
	materialSysNode->InsertEndChild( diffuse_elem );
	
	// Save the materials specular color
	stringstream spe_x;
	spe_x << material->SpecularColor().x;
	stringstream spe_y;
	spe_y << material->SpecularColor().y;
	stringstream spe_z;
	spe_z << material->SpecularColor().z;
	stringstream spe_w;
	spe_w << material->SpecularColor().w;
	string spe_str;
	spe_str.append(spe_x.str());
	spe_str.append(" ");
	spe_str.append(spe_y.str());
	spe_str.append(" ");
	spe_str.append(spe_z.str());
	spe_str.append(" ");
	spe_str.append(spe_w.str());
	XMLElement* specular_elem = doc->NewElement( "SpecularColor" );
	specular_elem->InsertFirstChild( doc->NewText(spe_str.c_str()) );
	materialSysNode->InsertEndChild( specular_elem );
	
	// Save the materials specular exponent
	stringstream sp;
	sp << material->SpecularPower();
	const char* specular_power_str = sp.str().c_str();
	XMLElement* specular_power_elem = doc->NewElement( "SpecularPower" );
	specular_power_elem->InsertFirstChild( doc->NewText(specular_power_str) );
	materialSysNode->InsertEndChild( specular_power_elem );
	
	// Save the materials specular intensity
	stringstream si;
	si << material->SpecularIntensity();
	const char* specular_intensity_str = si.str().c_str();
	XMLElement* specular_intensity_elem = doc->NewElement( "SpecularIntensity" );
	specular_intensity_elem->InsertFirstChild( doc->NewText(specular_intensity_str) );
	materialSysNode->InsertEndChild( specular_intensity_elem );
	
	// Save the materials emissive color
	stringstream emi;
	emi << material->Emissivity();
	const char* emissive_str = emi.str().c_str();
	XMLElement* emissive_elem = doc->NewElement( "Emissive" );
	emissive_elem->InsertFirstChild( doc->NewText(emissive_str) );
	materialSysNode->InsertEndChild( emissive_elem );
	
	// Save the materials reflectivity
	stringstream reflec;
	reflec << material->Reflectivity();
	const char* reflectivity_str = reflec.str().c_str();
	XMLElement* reflectivity_elem = doc->NewElement( "Reflectivity" );
	reflectivity_elem->InsertFirstChild( doc->NewText(reflectivity_str) );
	materialSysNode->InsertEndChild( reflectivity_elem );
	
	// Save the materials transmissivity
	stringstream transmiss;
	transmiss << material->Transmissivity();
	const char* transmissivity_str = transmiss.str().c_str();
	XMLElement* transmissivity_elem = doc->NewElement( "Transmissivity" );
	transmissivity_elem->InsertFirstChild( doc->NewText(transmissivity_str) );
	materialSysNode->InsertEndChild( transmissivity_elem );
	
	//Save the diffuse array texture names
	XMLElement* names_elem = doc->NewElement( "DiffuseArray" );
	for (int i = 0; i < material->GetDiffuseArrayTextureNames().size(); i++)
	{
		string tex_name = material->GetDiffuseArrayTextureNames()[i];
		
		XMLElement* texture_elem = doc->NewElement( "Texture" );
		
		//Save the id of the texture name
		texture_elem->SetAttribute("id", i);
		
		XMLElement* tex_name_elem = doc->NewElement( "TexName" );
		tex_name_elem->InsertFirstChild( doc->NewText(tex_name.c_str()) );
		texture_elem->InsertEndChild( tex_name_elem );
		
		names_elem->InsertEndChild( texture_elem );
	}
	
	materialSysNode->InsertEndChild( names_elem );
	
	// Save the materials texture
	
	for (int i = 0; i < material->TextureCount(); i++)
	{
		Texture* tex = material->GetTexture(i);
		
		switch (material->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
			{
				const char* tex_str = tex->GetTextureName().c_str();
				XMLElement* tex_elem = doc->NewElement( "DiffuseTexture" );
				tex_elem->InsertFirstChild( doc->NewText(tex_str) );
				materialSysNode->InsertEndChild( tex_elem );
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
			{
				const char* tex_str = tex->GetTextureName().c_str();
				XMLElement* tex_elem = doc->NewElement( "DetailTexture" );
				tex_elem->InsertFirstChild( doc->NewText(tex_str) );
				materialSysNode->InsertEndChild( tex_elem );
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
			{
				const char* tex_str = tex->GetTextureName().c_str();
				XMLElement* tex_elem = doc->NewElement( "NormalTexture" );
				tex_elem->InsertFirstChild( doc->NewText(tex_str) );
				materialSysNode->InsertEndChild( tex_elem );
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
			{
				const char* tex_str = tex->GetTextureName().c_str();
				XMLElement* tex_elem = doc->NewElement( "BlendTexture" );
				tex_elem->InsertFirstChild( doc->NewText(tex_str) );
				materialSysNode->InsertEndChild( tex_elem );
			}
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAlpha:
			{
				const char* tex_str = tex->GetTextureName().c_str();
				XMLElement* tex_elem = doc->NewElement( "AlphaTexture" );
				tex_elem->InsertFirstChild( doc->NewText(tex_str) );
				materialSysNode->InsertEndChild( tex_elem );
			}
			break;
		}
	}
	
	string fname;
	fname.append(filename);
	doc->SaveFile( fname.c_str() );
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveHUDXML(string filename, HUDSystem* hs)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	//We only want to save the currently rendered hud to its own file

	XMLNode* hudSysNode = doc->InsertEndChild( doc->NewElement( "HUD" ) );

	char buffer[1024];
	//HUD* h = new HUD();
	int i = 0;
	HUD* hud = hs->GetHUD();

	//Save the size of the menu
	int hud_x = hud->GetWidth();
	int hud_y = hud->GetHeight();
	stringstream ssx;
	ssx << hud_x;
	stringstream ssy;
	ssy << hud_y;
	string hud_size;
	hud_size.append(ssx.str());
	hud_size.append("x");
	hud_size.append(ssy.str());
	XMLElement* hud_size_elem = doc->NewElement( "hud_size" );
	hud_size_elem->InsertFirstChild( doc->NewText(hud_size.c_str()) );
	hudSysNode->InsertEndChild( hud_size_elem );

	//Save the images in the hud
	XMLElement* images_elem = doc->NewElement( "Images" );
	for (int i = 0; i < hud->GetImages().size(); i++)
	{
		ZShadeSandboxGraphics::Image* im = hud->GetImages()[i];

		XMLElement* image_elem = doc->NewElement( "Image" );

		//Save the id of the button
		image_elem->SetAttribute("id", i);

		//Save the button x pos
		memset(buffer, 0, sizeof(buffer));
		_itoa(im->GetXPos(), buffer, 10);
		string image_x_str = string(buffer);
		XMLElement* image_x_elem = doc->NewElement( "image_x" );
		image_x_elem->InsertFirstChild( doc->NewText(image_x_str.c_str()) );
		image_elem->InsertEndChild( image_x_elem );

		//Save the button y pos
		memset(buffer, 0, sizeof(buffer));
		_itoa(im->GetYPos(), buffer, 10);
		string image_y_str = string(buffer);
		XMLElement* image_y_elem = doc->NewElement( "image_y" );
		image_y_elem->InsertFirstChild( doc->NewText(image_y_str.c_str()) );
		image_elem->InsertEndChild( image_y_elem );

		//Save the button width
		memset(buffer, 0, sizeof(buffer));
		_itoa(im->GetWidth(), buffer, 10);
		string image_width_str = string(buffer);
		XMLElement* image_width_elem = doc->NewElement( "image_width" );
		image_width_elem->InsertFirstChild( doc->NewText(image_width_str.c_str()) );
		image_elem->InsertEndChild( image_width_elem );

		//Save the button height
		memset(buffer, 0, sizeof(buffer));
		_itoa(im->GetHeight(), buffer, 10);
		string image_height_str = string(buffer);
		XMLElement* image_height_elem = doc->NewElement( "image_height" );
		image_height_elem->InsertFirstChild( doc->NewText(image_height_str.c_str()) );
		image_elem->InsertEndChild( image_height_elem );

		//Save the button text
		XMLElement* image_text_elem = doc->NewElement( "image_name" );
		image_text_elem->InsertFirstChild( doc->NewText(im->GetName().c_str()) );
		image_elem->InsertEndChild( image_text_elem );
		
		images_elem->InsertEndChild( image_elem );
	}
	
	hudSysNode->InsertEndChild( images_elem );

	//Save the buttons in the menu
	XMLElement* texts_elem = doc->NewElement( "Texts" );
	for (int i = 0; i < hud->GetTexts().size(); i++)
	{
		ZShadeSandboxGraphics::Text* t = hud->GetTexts()[i];

		XMLElement* text_elem = doc->NewElement( "Text" );

		//Save the id of the text
		text_elem->SetAttribute("id", i);

		//Save the text x pos
		memset(buffer, 0, sizeof(buffer));
		_itoa(t->GetXPos(), buffer, 10);
		string text_x_str = string(buffer);
		XMLElement* text_x_elem = doc->NewElement( "text_x" );
		text_x_elem->InsertFirstChild( doc->NewText(text_x_str.c_str()) );
		text_elem->InsertEndChild( text_x_elem );

		//Save the text y pos
		memset(buffer, 0, sizeof(buffer));
		_itoa(t->GetYPos(), buffer, 10);
		string text_y_str = string(buffer);
		XMLElement* text_y_elem = doc->NewElement( "text_y" );
		text_y_elem->InsertFirstChild( doc->NewText(text_y_str.c_str()) );
		text_elem->InsertEndChild( text_y_elem );

		//Save the text width
		//memset(buffer, 0, sizeof(buffer));
		//itoa(t->GetWidth(), buffer, 10);
		//string text_width_str = string(buffer);
		//XMLElement* text_width_elem = doc->NewElement( "text_width" );
		//text_width_elem->InsertFirstChild( doc->NewText(text_width_str.c_str()) );
		//text_elem->InsertEndChild( text_width_elem );

		//Save the text height
		//memset(buffer, 0, sizeof(buffer));
		//itoa(t->GetHeight(), buffer, 10);
		//string text_height_str = string(buffer);
		//XMLElement* text_height_elem = doc->NewElement( "text_height" );
		//text_height_elem->InsertFirstChild( doc->NewText(text_height_str.c_str()) );
		//text_elem->InsertEndChild( text_height_elem );

		//Save the text for the text
		XMLElement* text_text_elem = doc->NewElement( "text" );
		text_text_elem->InsertFirstChild( doc->NewText(t->TextName().toString().c_str()) );
		text_elem->InsertEndChild( text_text_elem );

		//Save the text font
		memset(buffer, 0, sizeof(buffer));
		_itoa(t->FontSize(), buffer, 10);
		string text_font_str = string(buffer);
		XMLElement* text_font_elem = doc->NewElement( "text_font" );
		text_font_elem->InsertFirstChild( doc->NewText(text_font_str.c_str()) );
		text_elem->InsertEndChild( text_font_elem );

		//color
		//ZRGB z_rgb = t->GetColor10();
		XMFLOAT4 tColor = t->TextColor();
		stringstream ssr;
		ssr << tColor.x;
		stringstream ssg;
		ssg << tColor.y;
		stringstream ssb;
		ssb << tColor.z;
		string t_color;
		t_color.append(ssr.str());
		t_color.append(" ");
		t_color.append(ssg.str());
		t_color.append(" ");
		t_color.append(ssb.str());
		XMLElement* text_color_elem = doc->NewElement( "text_color" );
		text_color_elem->InsertFirstChild( doc->NewText(t_color.c_str()) );
		text_elem->InsertEndChild( text_color_elem );
		
		//Does the text have a shadow
		int has_shadow = t->HasShadow() ? 1 : 0;
		memset(buffer, 0, sizeof(buffer));
		_itoa(has_shadow, buffer, 10);
		string text_shadow_str = string(buffer);
		XMLElement* text_shadow_elem = doc->NewElement("shadow");
		text_shadow_elem->InsertFirstChild(doc->NewText(text_shadow_str.c_str()));
		text_elem->InsertEndChild(text_shadow_elem);

		//background
		XMLElement* text_optional_background_elem = doc->NewElement( "optional_background" );
		text_optional_background_elem->InsertFirstChild( doc->NewText("NONE"));//t->GetOptionalImageName().c_str()) );
		text_elem->InsertEndChild( text_optional_background_elem );

		texts_elem->InsertEndChild( text_elem );
	}
	
	hudSysNode->InsertEndChild( texts_elem );

	string fname;
	fname.append(filename);
	doc->SaveFile( fname.c_str() );

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveWorldXML(SWorld* sworld, string filename)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* mapNode = doc->InsertEndChild( doc->NewElement( "World" ) );

	//Create the world xml filename

	XMLElement* name_elem = doc->NewElement( "name" );
	name_elem->InsertFirstChild(doc->NewText(sworld->sWorldName.c_str()));
	mapNode->InsertEndChild( name_elem );

	XMLElement* maps_elem = doc->NewElement("Maps");

	for (int i = 0; i < sworld->m_mapNames.size(); i++)
	{
		string map_name = sworld->m_mapNames[i];

		XMLElement* map_elem = doc->NewElement("Map");

		//Save the id of the map
		map_elem->SetAttribute("id", i);

		// Save the name of the map
		XMLElement* map_name_elem = doc->NewElement("map_name");
		map_name_elem->InsertFirstChild(doc->NewText(map_name.c_str()));
		map_elem->InsertEndChild(map_name_elem);

		maps_elem->InsertEndChild(map_elem);
	}

	mapNode->InsertEndChild(maps_elem);

	string fname;

	fname.append(filename);

	doc->SaveFile(fname.c_str());

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveWorldXML(SPlatformerWorld* sworld, string filename)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* mapNode = doc->InsertEndChild(doc->NewElement("World"));

	//Create the world xml filename

	XMLElement* name_elem = doc->NewElement("name");
	name_elem->InsertFirstChild(doc->NewText(sworld->sWorldName.c_str()));
	mapNode->InsertEndChild(name_elem);

	XMLElement* maps_elem = doc->NewElement("Maps");

	for (int i = 0; i < sworld->m_mapNames.size(); i++)
	{
		string map_name = sworld->m_mapNames[i];

		XMLElement* map_elem = doc->NewElement("Map");

		//Save the id of the map
		map_elem->SetAttribute("id", i);

		// Save the name of the map
		XMLElement* map_name_elem = doc->NewElement("map_name");
		map_name_elem->InsertFirstChild(doc->NewText(map_name.c_str()));
		map_elem->InsertEndChild(map_name_elem);

		maps_elem->InsertEndChild(map_elem);
	}

	mapNode->InsertEndChild(maps_elem);

	string fname;

	fname.append(filename);

	doc->SaveFile(fname.c_str());

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveSpritesXML(TopdownMap* map, string spriteMapPath, string spriteMapfilename, HWND hwndPB)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* spritesNode = doc->InsertEndChild( doc->NewElement( "Sprites" ) );

	/*XMLElement* map_name_elem = doc->NewElement( "map_name" );
	map_name_elem->InsertFirstChild( doc->NewText("Map") );
	spritesNode->InsertEndChild( map_name_elem );*/

	char buffer[1024];
	AISprite* spr;
	//Loop through all the sprites in the map
	for (int i = 0; i < map->getMaxSprites(); i++)
	{
		spr = map->getSprite(i);

		XMLElement* sprite_elem = doc->NewElement( "Sprite" );

		//Save the id of the sprite
		sprite_elem->SetAttribute("id", i);

		if (spr->IsPlayer())
		{
			XMLElement* player_elem = doc->NewElement( "player" );
			player_elem->InsertFirstChild( doc->NewText("true") );
			sprite_elem->InsertEndChild( player_elem );
		}
		else
		{
			XMLElement* player_elem = doc->NewElement( "player" );
			player_elem->InsertFirstChild( doc->NewText("false") );
			sprite_elem->InsertEndChild( player_elem );
		}

		//Save the x position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TopLeftPosition().x, buffer, 10);
		string x_str = string(buffer);
		XMLElement* x_elem = doc->NewElement( "x" );
		x_elem->InsertFirstChild( doc->NewText(x_str.c_str()) );
		sprite_elem->InsertEndChild( x_elem );

		//Save the y position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TopLeftPosition().y, buffer, 10);
		string y_str = string(buffer);
		XMLElement* y_elem = doc->NewElement( "y" );
		y_elem->InsertFirstChild( doc->NewText(y_str.c_str()) );
		sprite_elem->InsertEndChild( y_elem );

		//Save the width
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Width(), buffer, 10);
		string width_str = string(buffer);
		XMLElement* width_elem = doc->NewElement( "width" );
		width_elem->InsertFirstChild( doc->NewText(width_str.c_str()) );
		sprite_elem->InsertEndChild( width_elem );

		//Save the height
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Height(), buffer, 10);
		string height_str = string(buffer);
		XMLElement* height_elem = doc->NewElement( "height" );
		height_elem->InsertFirstChild( doc->NewText(height_str.c_str()) );
		sprite_elem->InsertEndChild( height_elem );

		float hb_x;
		float hb_y;
		int hb_width;
		int hb_height;
		spr->GetHardbox(hb_x, hb_y, hb_width, hb_height);

		//Save the hardbox x position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_x, buffer, 10);
		string hbox_x_str = string(buffer);
		XMLElement* hbox_x_elem = doc->NewElement( "hbox_x" );
		hbox_x_elem->InsertFirstChild( doc->NewText(hbox_x_str.c_str()) );
		sprite_elem->InsertEndChild( hbox_x_elem );

		//Save the hardbox y position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_y, buffer, 10);
		string hbox_y_str = string(buffer);
		XMLElement* hbox_y_elem = doc->NewElement( "hbox_y" );
		hbox_y_elem->InsertFirstChild( doc->NewText(hbox_y_str.c_str()) );
		sprite_elem->InsertEndChild( hbox_y_elem );

		//Save the hardbox width
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_width, buffer, 10);
		string hbox_width_str = string(buffer);
		XMLElement* hbox_width_elem = doc->NewElement( "hbox_width" );
		hbox_width_elem->InsertFirstChild( doc->NewText(hbox_width_str.c_str()) );
		sprite_elem->InsertEndChild( hbox_width_elem );

		//Save the hardbox height
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_height, buffer, 10);
		string hbox_height_str = string(buffer);
		XMLElement* hbox_height_elem = doc->NewElement( "hbox_height" );
		hbox_height_elem->InsertFirstChild( doc->NewText(hbox_height_str.c_str()) );
		sprite_elem->InsertEndChild( hbox_height_elem );

		//Save the baseimage
		XMLElement* baseimage_elem = doc->NewElement( "baseimage" );
		//string::size_type pos = string( spr->GetBaseTextureFilename() ).find_last_of( "\\/" );
		//string name = CGlobal::substring(spr->GetBaseTextureFilename(), pos + 1, sizeof(spr->GetBaseTextureFilename()));
		baseimage_elem->InsertFirstChild( doc->NewText(spr->BaseTextureFilename().c_str()) );
		sprite_elem->InsertEndChild( baseimage_elem );
		
		//Save the vision
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Vision(), buffer, 10);
		string vision_str = string(buffer);
		XMLElement* vision_elem = doc->NewElement( "vision" );
		vision_elem->InsertFirstChild( doc->NewText(vision_str.c_str()) );
		sprite_elem->InsertEndChild( vision_elem );

		//Save the depth
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Depth(), buffer, 10);
		string depth_str = string(buffer);
		XMLElement* depth_elem = doc->NewElement( "depth" );
		depth_elem->InsertFirstChild( doc->NewText(depth_str.c_str()) );
		sprite_elem->InsertEndChild( depth_elem );

		//Save the hardness
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsHard(), buffer, 10);
		string hard_str = string(buffer);
		XMLElement* hard_elem = doc->NewElement( "hard" );
		hard_elem->InsertFirstChild( doc->NewText(hard_str.c_str()) );
		sprite_elem->InsertEndChild( hard_elem );

		//Save the nohit
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsNoHit(), buffer, 10);
		string nohit_str = string(buffer);
		XMLElement* nohit_elem = doc->NewElement( "nohit" );
		nohit_elem->InsertFirstChild( doc->NewText(nohit_str.c_str()) );
		sprite_elem->InsertEndChild( nohit_elem );

		//Save the animation profile name
		XMLElement* animation_elem = doc->NewElement( "animation" );
		animation_elem->InsertFirstChild( doc->NewText(spr->AnimationProfileName().c_str()) );
		sprite_elem->InsertEndChild( animation_elem );

		//Save the default sequence
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->DefaultSequence(), buffer, 10);
		string default_seq_str = string(buffer);
		XMLElement* default_seq_elem = doc->NewElement( "default_seq" );
		default_seq_elem->InsertFirstChild( doc->NewText(default_seq_str.c_str()) );
		sprite_elem->InsertEndChild( default_seq_elem );

		//Save the anim speed
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->AnimationSpeed(), buffer, 10);
		string anim_speed_str = string(buffer);
		XMLElement* anim_speed_elem = doc->NewElement( "anim_speed" );
		anim_speed_elem->InsertFirstChild( doc->NewText(anim_speed_str.c_str()) );
		sprite_elem->InsertEndChild( anim_speed_elem );

		//Save the hp
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->HP(), buffer, 10);
		string hp_str = string(buffer);
		XMLElement* hp_elem = doc->NewElement( "hp" );
		hp_elem->InsertFirstChild( doc->NewText(hp_str.c_str()) );
		sprite_elem->InsertEndChild( hp_elem );

		//Save the hp max
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->HPMax(), buffer, 10);
		string hp_max_str = string(buffer);
		XMLElement* hp_max_elem = doc->NewElement( "hp_max" );
		hp_max_elem->InsertFirstChild( doc->NewText(hp_max_str.c_str()) );
		sprite_elem->InsertEndChild( hp_max_elem );

		//Save the mana
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Mana(), buffer, 10);
		string mana_str = string(buffer);
		XMLElement* mana_elem = doc->NewElement( "mana" );
		mana_elem->InsertFirstChild( doc->NewText(mana_str.c_str()) );
		sprite_elem->InsertEndChild( mana_elem );

		//Save the gold
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Gold(), buffer, 10);
		string gold_str = string(buffer);
		XMLElement* gold_elem = doc->NewElement( "gold" );
		gold_elem->InsertFirstChild( doc->NewText(gold_str.c_str()) );
		sprite_elem->InsertEndChild( gold_elem );

		//Save the strength
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Strength(), buffer, 10);
		string strength_str = string(buffer);
		XMLElement* strength_elem = doc->NewElement( "strength" );
		strength_elem->InsertFirstChild( doc->NewText(strength_str.c_str()) );
		sprite_elem->InsertEndChild( strength_elem );

		//Save the speed
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Speed(), buffer, 10);
		string speed_str = string(buffer);
		XMLElement* speed_elem = doc->NewElement( "speed" );
		speed_elem->InsertFirstChild( doc->NewText(speed_str.c_str()) );
		sprite_elem->InsertEndChild( speed_elem );

		//Save the defense
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Defense(), buffer, 10);
		string defense_str = string(buffer);
		XMLElement* defense_elem = doc->NewElement( "defense" );
		defense_elem->InsertFirstChild( doc->NewText(defense_str.c_str()) );
		sprite_elem->InsertEndChild( defense_elem );

		//Save the experience
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Experience(), buffer, 10);
		string experience_str = string(buffer);
		XMLElement* experience_elem = doc->NewElement( "experience" );
		experience_elem->InsertFirstChild( doc->NewText(experience_str.c_str()) );
		sprite_elem->InsertEndChild( experience_elem );

		//Save the level
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Level(), buffer, 10);
		string level_str = string(buffer);
		XMLElement* level_elem = doc->NewElement( "level" );
		level_elem->InsertFirstChild( doc->NewText(level_str.c_str()) );
		sprite_elem->InsertEndChild( level_elem );

		//Save the script
		XMLElement* script_elem = doc->NewElement( "script" );
		script_elem->InsertFirstChild( doc->NewText(spr->ScriptName().c_str()) );
		sprite_elem->InsertEndChild( script_elem );

		//Save the touch
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsTouch(), buffer, 10);
		string touch_str = string(buffer);
		XMLElement* touch_elem = doc->NewElement( "touch" );
		touch_elem->InsertFirstChild( doc->NewText(touch_str.c_str()) );
		sprite_elem->InsertEndChild( touch_elem );

		//Save the background
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsBackgroundSprite(), buffer, 10);
		string background_str = string(buffer);
		XMLElement* background_elem = doc->NewElement( "background" );
		background_elem->InsertFirstChild( doc->NewText(background_str.c_str()) );
		sprite_elem->InsertEndChild( background_elem );

		//Save the entity
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsEntitySprite(), buffer, 10);
		string entity_str = string(buffer);
		XMLElement* entity_elem = doc->NewElement( "entity" );
		entity_elem->InsertFirstChild( doc->NewText(entity_str.c_str()) );
		sprite_elem->InsertEndChild( entity_elem );

		//Save the invisible
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsInvisibleSprite(), buffer, 10);
		string invisible_str = string(buffer);
		XMLElement* invisible_elem = doc->NewElement( "invisible" );
		invisible_elem->InsertFirstChild( doc->NewText(invisible_str.c_str()) );
		sprite_elem->InsertEndChild( invisible_elem );

		//Save the sprite dynamic or static
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsStatic(), buffer, 10);
		string is_static_str = string(buffer);
		XMLElement* is_static_elem = doc->NewElement( "static" );
		is_static_elem->InsertFirstChild( doc->NewText(is_static_str.c_str()) );
		sprite_elem->InsertEndChild( is_static_elem );

		//Save the name
		XMLElement* name_elem = doc->NewElement( "name" );
		name_elem->InsertFirstChild( doc->NewText(spr->Name().c_str()) );
		sprite_elem->InsertEndChild( name_elem );

		//Save the collide msg
		XMLElement* collide_msg_elem = doc->NewElement( "collide_msg" );
		collide_msg_elem->InsertFirstChild( doc->NewText(spr->CollideMsg().c_str()) );
		sprite_elem->InsertEndChild( collide_msg_elem );

		//Save the teleport
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->CanTeleport(), buffer, 10);
		string teleport_str = string(buffer);
		XMLElement* teleport_elem = doc->NewElement( "teleport" );
		teleport_elem->InsertFirstChild( doc->NewText(teleport_str.c_str()) );
		sprite_elem->InsertEndChild( teleport_elem );

		//Save the teleport map name
		XMLElement* teleport_map_elem = doc->NewElement( "teleport_map" );
		teleport_map_elem->InsertFirstChild( doc->NewText(spr->TeleportMapName().c_str()) );
		sprite_elem->InsertEndChild( teleport_map_elem );

		//Save the teleport map x
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TeleportX(), buffer, 10);
		string teleport_map_x_str = string(buffer);
		XMLElement* teleport_map_x_elem = doc->NewElement( "teleport_map_x" );
		teleport_map_x_elem->InsertFirstChild( doc->NewText(teleport_map_x_str.c_str()) );
		sprite_elem->InsertEndChild( teleport_map_x_elem );

		//Save the teleport map y
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TeleportY(), buffer, 10);
		string teleport_map_y_str = string(buffer);
		XMLElement* teleport_map_y_elem = doc->NewElement( "teleport_map_y" );
		teleport_map_y_elem->InsertFirstChild( doc->NewText(teleport_map_y_str.c_str()) );
		sprite_elem->InsertEndChild( teleport_map_y_elem );

		//Add the sprite into the sprites node
		spritesNode->InsertEndChild( sprite_elem );

		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
	}

	string fname;
	fname.append(spriteMapfilename);
	doc->SaveFile( fname.c_str() );

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveSpritesXML(PlatformerMap* map, string spriteMapPath, string spriteMapfilename, HWND hwndPB)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* spritesNode = doc->InsertEndChild(doc->NewElement("Sprites"));

	/*XMLElement* map_name_elem = doc->NewElement( "map_name" );
	map_name_elem->InsertFirstChild( doc->NewText("Map") );
	spritesNode->InsertEndChild( map_name_elem );*/

	char buffer[1024];
	AISprite* spr;

	//Loop through all the sprites in the map
	for (int i = 0; i < map->NumSprites(); i++)
	{
		spr = map->getSprite(i);

		XMLElement* sprite_elem = doc->NewElement("Sprite");

		//Save the id of the sprite
		sprite_elem->SetAttribute("id", i);

		if (spr->IsPlayer())
		{
			XMLElement* player_elem = doc->NewElement("player");
			player_elem->InsertFirstChild(doc->NewText("true"));
			sprite_elem->InsertEndChild(player_elem);
		}
		else
		{
			XMLElement* player_elem = doc->NewElement("player");
			player_elem->InsertFirstChild(doc->NewText("false"));
			sprite_elem->InsertEndChild(player_elem);
		}

		//Save the x position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TopLeftPosition().x, buffer, 10);
		string x_str = string(buffer);
		XMLElement* x_elem = doc->NewElement("x");
		x_elem->InsertFirstChild(doc->NewText(x_str.c_str()));
		sprite_elem->InsertEndChild(x_elem);

		//Save the y position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TopLeftPosition().y, buffer, 10);
		string y_str = string(buffer);
		XMLElement* y_elem = doc->NewElement("y");
		y_elem->InsertFirstChild(doc->NewText(y_str.c_str()));
		sprite_elem->InsertEndChild(y_elem);

		//Save the width
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Width(), buffer, 10);
		string width_str = string(buffer);
		XMLElement* width_elem = doc->NewElement("width");
		width_elem->InsertFirstChild(doc->NewText(width_str.c_str()));
		sprite_elem->InsertEndChild(width_elem);

		//Save the height
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Height(), buffer, 10);
		string height_str = string(buffer);
		XMLElement* height_elem = doc->NewElement("height");
		height_elem->InsertFirstChild(doc->NewText(height_str.c_str()));
		sprite_elem->InsertEndChild(height_elem);

		float hb_x;
		float hb_y;
		int hb_width;
		int hb_height;
		spr->GetHardbox(hb_x, hb_y, hb_width, hb_height);

		//Save the hardbox x position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_x, buffer, 10);
		string hbox_x_str = string(buffer);
		XMLElement* hbox_x_elem = doc->NewElement("hbox_x");
		hbox_x_elem->InsertFirstChild(doc->NewText(hbox_x_str.c_str()));
		sprite_elem->InsertEndChild(hbox_x_elem);

		//Save the hardbox y position
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_y, buffer, 10);
		string hbox_y_str = string(buffer);
		XMLElement* hbox_y_elem = doc->NewElement("hbox_y");
		hbox_y_elem->InsertFirstChild(doc->NewText(hbox_y_str.c_str()));
		sprite_elem->InsertEndChild(hbox_y_elem);

		//Save the hardbox width
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_width, buffer, 10);
		string hbox_width_str = string(buffer);
		XMLElement* hbox_width_elem = doc->NewElement("hbox_width");
		hbox_width_elem->InsertFirstChild(doc->NewText(hbox_width_str.c_str()));
		sprite_elem->InsertEndChild(hbox_width_elem);

		//Save the hardbox height
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(hb_height, buffer, 10);
		string hbox_height_str = string(buffer);
		XMLElement* hbox_height_elem = doc->NewElement("hbox_height");
		hbox_height_elem->InsertFirstChild(doc->NewText(hbox_height_str.c_str()));
		sprite_elem->InsertEndChild(hbox_height_elem);

		//Save the baseimage
		XMLElement* baseimage_elem = doc->NewElement("baseimage");
		//string::size_type pos = string( spr->GetBaseTextureFilename() ).find_last_of( "\\/" );
		//string name = CGlobal::substring(spr->GetBaseTextureFilename(), pos + 1, sizeof(spr->GetBaseTextureFilename()));
		baseimage_elem->InsertFirstChild(doc->NewText(spr->BaseTextureFilename().c_str()));
		sprite_elem->InsertEndChild(baseimage_elem);

		//Save the hardness
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsHard(), buffer, 10);
		string hard_str = string(buffer);
		XMLElement* hard_elem = doc->NewElement("hard");
		hard_elem->InsertFirstChild(doc->NewText(hard_str.c_str()));
		sprite_elem->InsertEndChild(hard_elem);

		//Save the nohit
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsNoHit(), buffer, 10);
		string nohit_str = string(buffer);
		XMLElement* nohit_elem = doc->NewElement("nohit");
		nohit_elem->InsertFirstChild(doc->NewText(nohit_str.c_str()));
		sprite_elem->InsertEndChild(nohit_elem);

		//Save the animation profile name
		XMLElement* animation_elem = doc->NewElement("animation");
		animation_elem->InsertFirstChild(doc->NewText(spr->AnimationProfileName().c_str()));
		sprite_elem->InsertEndChild(animation_elem);

		//Save the default sequence
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->DefaultSequence(), buffer, 10);
		string default_seq_str = string(buffer);
		XMLElement* default_seq_elem = doc->NewElement("default_seq");
		default_seq_elem->InsertFirstChild(doc->NewText(default_seq_str.c_str()));
		sprite_elem->InsertEndChild(default_seq_elem);

		//Save the anim speed
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->AnimationSpeed(), buffer, 10);
		string anim_speed_str = string(buffer);
		XMLElement* anim_speed_elem = doc->NewElement("anim_speed");
		anim_speed_elem->InsertFirstChild(doc->NewText(anim_speed_str.c_str()));
		sprite_elem->InsertEndChild(anim_speed_elem);

		//Save the hp
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->HP(), buffer, 10);
		string hp_str = string(buffer);
		XMLElement* hp_elem = doc->NewElement("hp");
		hp_elem->InsertFirstChild(doc->NewText(hp_str.c_str()));
		sprite_elem->InsertEndChild(hp_elem);

		//Save the hp max
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->HPMax(), buffer, 10);
		string hp_max_str = string(buffer);
		XMLElement* hp_max_elem = doc->NewElement("hp_max");
		hp_max_elem->InsertFirstChild(doc->NewText(hp_max_str.c_str()));
		sprite_elem->InsertEndChild(hp_max_elem);

		//Save the coin
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Coin(), buffer, 10);
		string coin_str = string(buffer);
		XMLElement* coin_elem = doc->NewElement("coin");
		coin_elem->InsertFirstChild(doc->NewText(coin_str.c_str()));
		sprite_elem->InsertEndChild(coin_elem);

		//Save the strength
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Strength(), buffer, 10);
		string strength_str = string(buffer);
		XMLElement* strength_elem = doc->NewElement("strength");
		strength_elem->InsertFirstChild(doc->NewText(strength_str.c_str()));
		sprite_elem->InsertEndChild(strength_elem);

		//Save the speed
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->Speed(), buffer, 10);
		string speed_str = string(buffer);
		XMLElement* speed_elem = doc->NewElement("speed");
		speed_elem->InsertFirstChild(doc->NewText(speed_str.c_str()));
		sprite_elem->InsertEndChild(speed_elem);

		////Save the mana
		//memset(buffer, 0, sizeof(buffer));
		//_itoa_s(spr->Mana(), buffer, 10);
		//string mana_str = string(buffer);
		//XMLElement* mana_elem = doc->NewElement("mana");
		//mana_elem->InsertFirstChild(doc->NewText(mana_str.c_str()));
		//sprite_elem->InsertEndChild(mana_elem);

		////Save the defense
		//memset(buffer, 0, sizeof(buffer));
		//_itoa_s(spr->Defense(), buffer, 10);
		//string defense_str = string(buffer);
		//XMLElement* defense_elem = doc->NewElement("defense");
		//defense_elem->InsertFirstChild(doc->NewText(defense_str.c_str()));
		//sprite_elem->InsertEndChild(defense_elem);

		////Save the experience
		//memset(buffer, 0, sizeof(buffer));
		//_itoa_s(spr->Experience(), buffer, 10);
		//string experience_str = string(buffer);
		//XMLElement* experience_elem = doc->NewElement("experience");
		//experience_elem->InsertFirstChild(doc->NewText(experience_str.c_str()));
		//sprite_elem->InsertEndChild(experience_elem);

		////Save the level
		//memset(buffer, 0, sizeof(buffer));
		//_itoa_s(spr->Level(), buffer, 10);
		//string level_str = string(buffer);
		//XMLElement* level_elem = doc->NewElement("level");
		//level_elem->InsertFirstChild(doc->NewText(level_str.c_str()));
		//sprite_elem->InsertEndChild(level_elem);

		//Save the script
		XMLElement* script_elem = doc->NewElement("script");
		script_elem->InsertFirstChild(doc->NewText(spr->ScriptName().c_str()));
		sprite_elem->InsertEndChild(script_elem);

		//Save the touch
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsTouch(), buffer, 10);
		string touch_str = string(buffer);
		XMLElement* touch_elem = doc->NewElement("touch");
		touch_elem->InsertFirstChild(doc->NewText(touch_str.c_str()));
		sprite_elem->InsertEndChild(touch_elem);

		//Save the background
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsBackgroundSprite(), buffer, 10);
		string background_str = string(buffer);
		XMLElement* background_elem = doc->NewElement("background");
		background_elem->InsertFirstChild(doc->NewText(background_str.c_str()));
		sprite_elem->InsertEndChild(background_elem);

		//Save the entity
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsEntitySprite(), buffer, 10);
		string entity_str = string(buffer);
		XMLElement* entity_elem = doc->NewElement("entity");
		entity_elem->InsertFirstChild(doc->NewText(entity_str.c_str()));
		sprite_elem->InsertEndChild(entity_elem);

		//Save the invisible
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsInvisibleSprite(), buffer, 10);
		string invisible_str = string(buffer);
		XMLElement* invisible_elem = doc->NewElement("invisible");
		invisible_elem->InsertFirstChild(doc->NewText(invisible_str.c_str()));
		sprite_elem->InsertEndChild(invisible_elem);

		//Save the sprite dynamic or static
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->IsStatic(), buffer, 10);
		string is_static_str = string(buffer);
		XMLElement* is_static_elem = doc->NewElement("static");
		is_static_elem->InsertFirstChild(doc->NewText(is_static_str.c_str()));
		sprite_elem->InsertEndChild(is_static_elem);

		//Save the name
		XMLElement* name_elem = doc->NewElement("name");
		name_elem->InsertFirstChild(doc->NewText(spr->Name().c_str()));
		sprite_elem->InsertEndChild(name_elem);

		//Save the collide msg
		XMLElement* collide_msg_elem = doc->NewElement("collide_msg");
		collide_msg_elem->InsertFirstChild(doc->NewText(spr->CollideMsg().c_str()));
		sprite_elem->InsertEndChild(collide_msg_elem);

		//Save the teleport
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->CanTeleport(), buffer, 10);
		string teleport_str = string(buffer);
		XMLElement* teleport_elem = doc->NewElement("teleport");
		teleport_elem->InsertFirstChild(doc->NewText(teleport_str.c_str()));
		sprite_elem->InsertEndChild(teleport_elem);

		//Save the teleport map name
		XMLElement* teleport_map_elem = doc->NewElement("teleport_map");
		teleport_map_elem->InsertFirstChild(doc->NewText(spr->TeleportMapName().c_str()));
		sprite_elem->InsertEndChild(teleport_map_elem);

		//Save the teleport map x
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TeleportX(), buffer, 10);
		string teleport_map_x_str = string(buffer);
		XMLElement* teleport_map_x_elem = doc->NewElement("teleport_map_x");
		teleport_map_x_elem->InsertFirstChild(doc->NewText(teleport_map_x_str.c_str()));
		sprite_elem->InsertEndChild(teleport_map_x_elem);

		//Save the teleport map y
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(spr->TeleportY(), buffer, 10);
		string teleport_map_y_str = string(buffer);
		XMLElement* teleport_map_y_elem = doc->NewElement("teleport_map_y");
		teleport_map_y_elem->InsertFirstChild(doc->NewText(teleport_map_y_str.c_str()));
		sprite_elem->InsertEndChild(teleport_map_y_elem);

		//Add the sprite into the sprites node
		spritesNode->InsertEndChild(sprite_elem);

		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
	}

	BetterString str(spriteMapPath);
	str += "\\";
	str += spriteMapfilename;

	string fname;
	fname.append(str.toString());
	doc->SaveFile(fname.c_str());

	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMapXML(TopdownMap* map, string filename, GameDirectory2D* gd, HWND hwndPB)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* mapNode = doc->InsertEndChild( doc->NewElement( "Map" ) );

	//Create the sprite xml filename

	//Append Sprites to the end of the name
	//Remove the .xml and add Sprites
	//BetterString str(filename);

	//int index = str.get_index('.');
	//BetterString xml_sprite_str = str.substring(0, index - 1);
	//xml_sprite_str += "Sprites.xml";
	
	BetterString str(filename);

	BetterString path = gd->m_xml_sprites_path;

	BetterString map_name = "";
	int indexI = 0;
	for (int i = 0; i < str.size() - 3; i++)
	{
		if (str[i] == 'M' && str[i + 1] == 'a' && str[i + 2] == 'p' && str[i + 3] == 's')
		{
			indexI = i + 5;
			break;
		}
	}

	int j = 0;
	for (int i = indexI; i < str.size(); i++)
	{
		if (str[i] == '.') break;
		map_name.insert_char(str[i], j++);
	}

	string xml_sprite_str = map_name.toString();
	xml_sprite_str += "Sprites.xml";

	//int index = str.get_index('.');
	//BetterString xml_sprite_str = str.substring(0, index);
	//BetterString xml_sprite_str = path;
	//xml_sprite_str += "\\";
	//xml_sprite_str += map_name;
	//xml_sprite_str += "Sprites.xml";

	/*
	int index = CGlobal::getIndex(filename, '.');
	string::size_type pos = string( filename ).find_last_of( "\\/" );
	string name = CGlobal::substring(filename, pos + 1, index);
	string path = string( filename ).substr( 0, pos - 4);
	path.append("Sprites\\");
	path.append(name);
	path.append("Sprites.xml");*
	string path = gd->m_xml_sprites_path;
	int index = CGlobal::getIndex(filename, '.');
	string::size_type pos = string( filename ).find_last_of( "\\/" );
	string name = CGlobal::substring(filename, pos + 1, index);
	path.append("\\");
	path.append(name);
	path.append("Sprites.xml");
	*/
	
	XMLElement* map_name_elem = doc->NewElement("map_name");
	map_name_elem->InsertFirstChild(doc->NewText(map->GetMapName().c_str()));
	mapNode->InsertEndChild(map_name_elem);

	XMLElement* xml_sprite_elem = doc->NewElement( "xml_sprite" );
	xml_sprite_elem->InsertFirstChild(doc->NewText(xml_sprite_str.c_str()));
	mapNode->InsertEndChild( xml_sprite_elem );

	string mapType;
	if (map->GetMapType() == Map2DType::REGULAR)
		mapType = "Regular";
	else if (map->GetMapType() == Map2DType::ISOMETRIC)
		mapType = "Isometric";

	XMLElement* map_type_elem = doc->NewElement("map_type");
	map_type_elem->InsertFirstChild(doc->NewText(mapType.c_str()));
	mapNode->InsertEndChild(map_type_elem);

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	_itoa_s(map->GetEngineOptions()->SECTION_ROW_SIZE, buffer, 10);
	string row_size_str = string(buffer);

	XMLElement* row_size_elem = doc->NewElement( "row_size" );
	row_size_elem->InsertFirstChild( doc->NewText(row_size_str.c_str()) );
	mapNode->InsertEndChild( row_size_elem );

	memset(buffer, 0, sizeof(buffer));
	_itoa_s(map->GetEngineOptions()->SECTION_COL_SIZE, buffer, 10);
	string col_size_str = string(buffer);

	XMLElement* col_size_elem = doc->NewElement( "col_size" );
	col_size_elem->InsertFirstChild( doc->NewText(col_size_str.c_str()) );
	mapNode->InsertEndChild( col_size_elem );

	XMLElement* fow_elem = doc->NewElement( "Fow" );
	fow_elem->SetAttribute("enabled", map->IsFOWEnabled());
	fow_elem->SetAttribute("radius", map->GetFOWRadius());
	fow_elem->SetAttribute("flashlight", map->IsFOWFlashlightEnabled());
	mapNode->InsertEndChild( fow_elem );

	//Loop through each section in the map
	vector<Section*> sections = map->GetSections();
	vector<Section*>::iterator it = sections.begin();
	int id = 0;
	for (; it != sections.end(); it++)
	{
		Section* section = (*it);

		XMLElement* sec_elem = doc->NewElement( "Section" );

		//Save the id of the section
		sec_elem->SetAttribute("id", id);

		//Save the vision
		memset(buffer, 0, sizeof(buffer));
		_itoa_s(section->GetVision(), buffer, 10);
		string vision_str = string(buffer);
		XMLElement* vision_elem = doc->NewElement( "Vision" );
		vision_elem->InsertFirstChild( doc->NewText(vision_str.c_str()) );
		sec_elem->InsertEndChild( vision_elem );

		//Save the music name
		XMLElement* music_elem = doc->NewElement( "Music" );
		if (BetterString(section->GetMusicName()) == "")
			music_elem->InsertFirstChild(doc->NewText("NONE"));
		else
			music_elem->InsertFirstChild(doc->NewText(section->GetMusicName().c_str()));
		sec_elem->InsertEndChild( music_elem );

		//Save the tiles in the section
		XMLElement* tiles_elem = doc->NewElement( "Tiles" );
		int tile_id = 0;
		vector<Tile*> tiles = section->GetTiles();
		vector<Tile*>::iterator tile_iter = tiles.begin();
		for (; tile_iter != tiles.end(); tile_iter++)
		{
			Tile* tile = (*tile_iter);
			
			XMLElement* tile_elem = doc->NewElement( "Tile" );

			tile_elem->SetAttribute("id", tile_id);
			
			if (tile->GetFogBit() == fow_non)
				tile_elem->SetAttribute("image", tile->GetTileTextureName().c_str());
			else
				tile_elem->SetAttribute("image", tile->GetTileTextureCopyName().c_str());
			
			//Save the animation profile name
			XMLElement* animation_elem = doc->NewElement( "animation" );
			animation_elem->InsertFirstChild( doc->NewText(tile->GetAnimationProfileName().c_str()) );
			tile_elem->InsertEndChild( animation_elem );
			
			//Save the anim speed
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(tile->GetAnimationSpeed(), buffer, 10);
			string anim_speed_str = string(buffer);
			XMLElement* anim_speed_elem = doc->NewElement( "anim_speed" );
			anim_speed_elem->InsertFirstChild( doc->NewText(anim_speed_str.c_str()) );
			tile_elem->InsertEndChild( anim_speed_elem );

			//Save the hardness
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(tile->IsHard(), buffer, 10);
			string hard_str = string(buffer);
			XMLElement* hard_elem = doc->NewElement( "hard" );
			hard_elem->InsertFirstChild( doc->NewText(hard_str.c_str()) );
			tile_elem->InsertEndChild( hard_elem );

			//Add the tile to the tiles node
			tiles_elem->InsertEndChild( tile_elem );
			tile_id++;
		}
		//Add the tiles to the section node
		sec_elem->InsertEndChild( tiles_elem );

		//Add the section to the map node
		mapNode->InsertEndChild( sec_elem );

		SendMessage(hwndPB, PBM_STEPIT, 0, 0);

		id++;
	}
	
	string fname;

	//fname.append("XML\\Maps\\");
	fname.append(filename);

	doc->SaveFile( fname.c_str() );

	//Write the sprites to the xml file
	SaveSpritesXML(map, gd->m_xml_sprites_path, xml_sprite_str, hwndPB);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMapXML(PlatformerMap* map, string filename, GameDirectory2D* gd, HWND hwndPB)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	XMLNode* mapNode = doc->InsertEndChild(doc->NewElement("Map"));

	//Create the sprite xml filename

	//Append Sprites to the end of the name
	//Remove the .xml and add Sprites
	BetterString str(filename);

	BetterString path = gd->m_xml_sprites_path;

	BetterString map_name = "";
	int indexI = 0;
	for (int i = 0; i < str.size() - 3; i++)
	{
		if (str[i] == 'M' && str[i + 1] == 'a' && str[i + 2] == 'p' && str[i + 3] == 's')
		{
			indexI = i + 5;
			break;
		}
	}

	int j = 0;
	for (int i = indexI; i < str.size(); i++)
	{
		if (str[i] == '.') break;
		map_name.insert_char(str[i], j++);
	}

	string xml_sprite_str = map_name.toString();
	xml_sprite_str += "Sprites.xml";

	//int index = str.get_index('.');
	//BetterString xml_sprite_str = str.substring(0, index);
	//BetterString xml_sprite_str = path;
	//xml_sprite_str += "\\";
	//xml_sprite_str += map_name;
	//xml_sprite_str += "Sprites.xml";

	XMLElement* map_name_elem = doc->NewElement("map_name");
	map_name_elem->InsertFirstChild(doc->NewText(map->MapName().c_str()));
	mapNode->InsertEndChild(map_name_elem);

	XMLElement* xml_sprite_elem = doc->NewElement("xml_sprite");
	xml_sprite_elem->InsertFirstChild(doc->NewText(xml_sprite_str.c_str()));
	mapNode->InsertEndChild(xml_sprite_elem);

	XMLElement* xml_music_elem = doc->NewElement("music");
	if (BetterString(map->MusicName()) == "")
		xml_music_elem->InsertFirstChild(doc->NewText("NONE"));
	else
		xml_music_elem->InsertFirstChild(doc->NewText(map->MusicName().c_str()));
	mapNode->InsertEndChild(xml_music_elem);

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	_itoa_s(map->Width(), buffer, 10);
	string row_size_str = string(buffer);

	XMLElement* row_size_elem = doc->NewElement("width");
	row_size_elem->InsertFirstChild(doc->NewText(row_size_str.c_str()));
	mapNode->InsertEndChild(row_size_elem);

	memset(buffer, 0, sizeof(buffer));
	_itoa_s(map->Height(), buffer, 10);
	string col_size_str = string(buffer);

	XMLElement* col_size_elem = doc->NewElement("height");
	col_size_elem->InsertFirstChild(doc->NewText(col_size_str.c_str()));
	mapNode->InsertEndChild(col_size_elem);

	//Loop through each tile in the map
	XMLElement* tiles_elem = doc->NewElement("Tiles");

	SmartArray2D<PlatformerTile*> tiles = map->Tiles();

	int tile_id = 0;

	for (int y = 0; y < map->Height(); y++)
	{
		for (int x = 0; x < map->Width(); x++)
		{
			PlatformerTile* tile = tiles(x, y);

			if (tile == NULL) continue;
			if (tile->GetBitmap() == NULL) continue;

			XMLElement* tile_elem = doc->NewElement("Tile");

			tile_elem->SetAttribute("id", tile_id);

			tile_elem->SetAttribute("image", tile->GetTileTextureName().c_str());

			int x_pos = PlatformerMap::TilesToPixels(tile->GetPosition().x);
			int y_pos = PlatformerMap::TilesToPixels(tile->GetPosition().y);

			//Save the x pos
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(x_pos, buffer, 10);
			string x_str = string(buffer);
			XMLElement* x_elem = doc->NewElement("x");
			x_elem->InsertFirstChild(doc->NewText(x_str.c_str()));
			tile_elem->InsertEndChild(x_elem);

			//Save the y pos
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(y_pos, buffer, 10);
			string y_str = string(buffer);
			XMLElement* y_elem = doc->NewElement("y");
			y_elem->InsertFirstChild(doc->NewText(y_str.c_str()));
			tile_elem->InsertEndChild(y_elem);

			//Save the animation profile name
			XMLElement* animation_elem = doc->NewElement("animation");
			animation_elem->InsertFirstChild(doc->NewText(tile->GetAnimationProfileName().c_str()));
			tile_elem->InsertEndChild(animation_elem);

			//Save the anim speed
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(tile->GetAnimationSpeed(), buffer, 10);
			string anim_speed_str = string(buffer);
			XMLElement* anim_speed_elem = doc->NewElement("anim_speed");
			anim_speed_elem->InsertFirstChild(doc->NewText(anim_speed_str.c_str()));
			tile_elem->InsertEndChild(anim_speed_elem);

			//Save the hardness
			memset(buffer, 0, sizeof(buffer));
			_itoa_s(tile->IsHard(), buffer, 10);
			string hard_str = string(buffer);
			XMLElement* hard_elem = doc->NewElement("hard");
			hard_elem->InsertFirstChild(doc->NewText(hard_str.c_str()));
			tile_elem->InsertEndChild(hard_elem);

			//Add the tile to the tiles node
			tiles_elem->InsertEndChild(tile_elem);
			tile_id++;
		}
	}

	//Add the section to the map node
	mapNode->InsertEndChild(tiles_elem);

	SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	string fname;

	fname.append(filename);

	doc->SaveFile(fname.c_str());

	//Write the sprites to the xml file
	SaveSpritesXML(map, gd->m_xml_sprites_path, xml_sprite_str, hwndPB);
	
	return true;
}
//===============================================================================================================================
bool ZShadeXMLSaver::SaveMenuXML(string menufilename, MenuSystem* ms)
{
	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();

	//We only want to save the currently rendered menu to its own file

	XMLNode* menuSysNode = doc->InsertEndChild( doc->NewElement( "Menu" ) );

	char buffer[1024];
	ZShadeSandboxEnvironment::Menu* m;
	int i = 0;
	//Loop through all the menus in the menu system
	map<string, ZShadeSandboxEnvironment::Menu*> menus = ms->GetMenus();

	string menu_render_name = ms->GetMenuRenderName();

	map<string, ZShadeSandboxEnvironment::Menu*>::iterator iter = menus.find(menu_render_name);

	//map<string, Menu*>::iterator iter = menus.begin();
	//for (; iter != menus.end(); iter++, i++)
	if (iter != menus.end())
	{
		m = (*iter).second;

		//XMLElement* menu_elem = doc->NewElement( "Menu" );

		//Save the id of the menu
		//menuSysNode->SetAttribute("id", i);

		//Save the menu name
		XMLElement* menu_name_elem = doc->NewElement( "menu_name" );
		menu_name_elem->InsertFirstChild( doc->NewText(m->GetMenuName().c_str()) );
		menuSysNode->InsertEndChild( menu_name_elem );

		//Save the size of the menu
		int menu_x = m->GetMenuWidth();
		int menu_y = m->GetMenuHeight();
		stringstream ssx;
		ssx << menu_x;
		stringstream ssy;
		ssy << menu_y;
		string menu_size;
		menu_size.append(ssx.str());
		menu_size.append("x");
		menu_size.append(ssy.str());
		XMLElement* menu_size_elem = doc->NewElement( "menu_size" );
		menu_size_elem->InsertFirstChild( doc->NewText(menu_size.c_str()) );
		menuSysNode->InsertEndChild( menu_size_elem );

		//Save the menu type
		string menu_type = "";
		if (m->GetMenuType() == ZShadeSandboxEnvironment::EMenuType::eMainMenu)
			menu_type = "MainMenu";
		else if (m->GetMenuType() == ZShadeSandboxEnvironment::EMenuType::eInGameMenu)
			menu_type = "InGameMenu";
		XMLElement* menu_type_elem = doc->NewElement( "menu_type" );
		menu_type_elem->InsertFirstChild( doc->NewText(menu_type.c_str()) );
		menuSysNode->InsertEndChild( menu_type_elem );

		//Save the menu background name
		XMLElement* menu_background_elem = doc->NewElement( "menu_background" );
		menu_background_elem->InsertFirstChild( doc->NewText(m->GetBackgroundName().c_str()) );
		menuSysNode->InsertEndChild( menu_background_elem );

		//Save the buttons in the menu
		XMLElement* buttons_elem = doc->NewElement( "Buttons" );
		for (int i = 0; i < m->GetButtons().size(); i++)
		{
			ZShadeSandboxGraphics::Button* b = m->GetButtons()[i];

			XMLElement* button_elem = doc->NewElement( "Button" );

			//Save the id of the button
			button_elem->SetAttribute("id", i);

			//Save the button x pos
			memset(buffer, 0, sizeof(buffer));
			_itoa(b->GetXPos(), buffer, 10);
			string button_x_str = string(buffer);
			XMLElement* button_x_elem = doc->NewElement( "button_x" );
			button_x_elem->InsertFirstChild( doc->NewText(button_x_str.c_str()) );
			button_elem->InsertEndChild( button_x_elem );

			//Save the button y pos
			memset(buffer, 0, sizeof(buffer));
			_itoa(b->GetYPos(), buffer, 10);
			string button_y_str = string(buffer);
			XMLElement* button_y_elem = doc->NewElement( "button_y" );
			button_y_elem->InsertFirstChild( doc->NewText(button_y_str.c_str()) );
			button_elem->InsertEndChild( button_y_elem );

			//Save the button width
			memset(buffer, 0, sizeof(buffer));
			_itoa(b->GetWidth(), buffer, 10);
			string button_width_str = string(buffer);
			XMLElement* button_width_elem = doc->NewElement( "button_width" );
			button_width_elem->InsertFirstChild( doc->NewText(button_width_str.c_str()) );
			button_elem->InsertEndChild( button_width_elem );

			//Save the button height
			memset(buffer, 0, sizeof(buffer));
			_itoa(b->GetHeight(), buffer, 10);
			string button_height_str = string(buffer);
			XMLElement* button_height_elem = doc->NewElement( "button_height" );
			button_height_elem->InsertFirstChild( doc->NewText(button_height_str.c_str()) );
			button_elem->InsertEndChild( button_height_elem );

			//Save the button text
			XMLElement* button_text_elem = doc->NewElement( "button_text" );
			button_text_elem->InsertFirstChild( doc->NewText(b->GetText().c_str()) );
			button_elem->InsertEndChild( button_text_elem );

			//Save the button normal image name
			XMLElement* button_normal_image_elem = doc->NewElement( "normal_image" );
			button_normal_image_elem->InsertFirstChild( doc->NewText(b->GetNormalImageName().c_str()) );
			button_elem->InsertEndChild( button_normal_image_elem );

			//Save the button pushed image name
			XMLElement* button_pushed_image_elem = doc->NewElement( "pushed_image" );
			button_pushed_image_elem->InsertFirstChild( doc->NewText(b->GetPushedImageName().c_str()) );
			button_elem->InsertEndChild( button_pushed_image_elem );

			//Save the button highlighted image name
			XMLElement* button_highlighted_image_elem = doc->NewElement( "highlighted_image" );
			button_highlighted_image_elem->InsertFirstChild( doc->NewText(b->GetHighlightedImageName().c_str()) );
			button_elem->InsertEndChild( button_highlighted_image_elem );

			//Save the button disabled image name
			XMLElement* button_disabled_image_elem = doc->NewElement( "disabled_image" );
			button_disabled_image_elem->InsertFirstChild( doc->NewText(b->GetDisabledImageName().c_str()) );
			button_elem->InsertEndChild( button_disabled_image_elem );

			//Save the button script type
			int st = b->GetScriptType();
			string st_name = "none";
			switch (st)
			{
				case ZShadeSandboxGraphics::EScriptType::eStart: st_name = "start"; break;
				case ZShadeSandboxGraphics::EScriptType::eResume: st_name = "resume"; break;
				case ZShadeSandboxGraphics::EScriptType::eContinue: st_name = "continue"; break;
				case ZShadeSandboxGraphics::EScriptType::eOptions: st_name = "options"; break;
				case ZShadeSandboxGraphics::EScriptType::eExit: st_name = "exit"; break;
				case ZShadeSandboxGraphics::EScriptType::eNone: st_name = "none"; break;
			}
			XMLElement* button_script_type_elem = doc->NewElement( "script_type" );
			button_script_type_elem->InsertFirstChild( doc->NewText(st_name.c_str()) );
			button_elem->InsertEndChild( button_script_type_elem );

			////Save the button action script name
			//XMLElement* button_action_script_elem = doc->NewElement( "action_script" );
			////Pull off the .lua
			//int index = CGlobal::getIndex(b->GetScript(), '.');
			//string filename = CGlobal::substring(b->GetScript(), 0, index);
			//button_action_script_elem->InsertFirstChild( doc->NewText(filename.c_str()) );
			//button_elem->InsertEndChild( button_action_script_elem );

			buttons_elem->InsertEndChild( button_elem );
		}
		menuSysNode->InsertEndChild( buttons_elem );

		//Save the buttons in the menu
		XMLElement* texts_elem = doc->NewElement( "Texts" );
		for (int i = 0; i < m->GetTexts().size(); i++)
		{
			ZShadeSandboxGraphics::Text* t = m->GetTexts()[i];

			XMLElement* text_elem = doc->NewElement( "Text" );

			//Save the id of the text
			text_elem->SetAttribute("id", i);

			//Save the text x pos
			memset(buffer, 0, sizeof(buffer));
			_itoa(t->GetXPos(), buffer, 10);
			string text_x_str = string(buffer);
			XMLElement* text_x_elem = doc->NewElement( "text_x" );
			text_x_elem->InsertFirstChild( doc->NewText(text_x_str.c_str()) );
			text_elem->InsertEndChild( text_x_elem );

			//Save the text y pos
			memset(buffer, 0, sizeof(buffer));
			_itoa(t->GetYPos(), buffer, 10);
			string text_y_str = string(buffer);
			XMLElement* text_y_elem = doc->NewElement( "text_y" );
			text_y_elem->InsertFirstChild( doc->NewText(text_y_str.c_str()) );
			text_elem->InsertEndChild( text_y_elem );

			////Save the text width
			//memset(buffer, 0, sizeof(buffer));
			//itoa(t->GetWidth(), buffer, 10);
			//string text_width_str = string(buffer);
			//XMLElement* text_width_elem = doc->NewElement( "text_width" );
			//text_width_elem->InsertFirstChild( doc->NewText(text_width_str.c_str()) );
			//text_elem->InsertEndChild( text_width_elem );

			////Save the text height
			//memset(buffer, 0, sizeof(buffer));
			//itoa(t->GetHeight(), buffer, 10);
			//string text_height_str = string(buffer);
			//XMLElement* text_height_elem = doc->NewElement( "text_height" );
			//text_height_elem->InsertFirstChild( doc->NewText(text_height_str.c_str()) );
			//text_elem->InsertEndChild( text_height_elem );

			//Save the text for the text
			XMLElement* text_text_elem = doc->NewElement( "text" );
			text_text_elem->InsertFirstChild( doc->NewText(t->TextName().toString().c_str()) );
			text_elem->InsertEndChild( text_text_elem );

			//Save the text font
			memset(buffer, 0, sizeof(buffer));
			_itoa(t->FontSize(), buffer, 10);
			string text_font_str = string(buffer);
			XMLElement* text_font_elem = doc->NewElement( "text_font" );
			text_font_elem->InsertFirstChild( doc->NewText(text_font_str.c_str()) );
			text_elem->InsertEndChild( text_font_elem );

			//color
			//ZRGB z_rgb = t->GetColor10();
			XMFLOAT4 tColor = t->TextColor();
			stringstream ssr;
			ssr << tColor.x;
			stringstream ssg;
			ssg << tColor.y;
			stringstream ssb;
			ssb << tColor.z;
			string t_color;
			t_color.append(ssr.str());
			t_color.append(" ");
			t_color.append(ssg.str());
			t_color.append(" ");
			t_color.append(ssb.str());
			XMLElement* text_color_elem = doc->NewElement( "text_color" );
			text_color_elem->InsertFirstChild( doc->NewText(t_color.c_str()) );
			text_elem->InsertEndChild( text_color_elem );

			//Does the text have a shadow
			int has_shadow = t->HasShadow() ? 1 : 0;
			memset(buffer, 0, sizeof(buffer));
			_itoa(has_shadow, buffer, 10);
			string text_shadow_str = string(buffer);
			XMLElement* text_shadow_elem = doc->NewElement("shadow");
			text_shadow_elem->InsertFirstChild(doc->NewText(text_shadow_str.c_str()));
			text_elem->InsertEndChild(text_shadow_elem);

			//background
			XMLElement* text_optional_background_elem = doc->NewElement( "optional_background" );
			text_optional_background_elem->InsertFirstChild(doc->NewText("NONE"));// t->GetOptionalImageName().c_str()) );
			text_elem->InsertEndChild( text_optional_background_elem );

			texts_elem->InsertEndChild( text_elem );
		}
		menuSysNode->InsertEndChild( texts_elem );

		//menuSysNode->InsertEndChild( menu_elem );
	}

	string fname;
	fname.append(menufilename);
	doc->SaveFile( fname.c_str() );

	return true;
}
//===============================================================================================================================