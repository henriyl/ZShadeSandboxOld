#include "DefaultMaterials.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
DefaultMaterials::DefaultMaterials()
{
}
//===============================================================================================================================
void DefaultMaterials::Build(D3D* d3d, string base_path)
{
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Target", "target.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "White", "white.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Black", "black.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Blue", "blue.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Red", "red.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Brick", "bricks.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Floor", "floor.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Grass", "grass.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Metal Panel", "metalpanel.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Soil", "soil.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Stone", "stone.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Snow", "snow.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Dirt", "dirt01.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Ice", "ice.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Metal", "metal001.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "Wall", "wall01.dds");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW All", "fow\\fow_all.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW CNE", "fow\\fow_CNE.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW CNW", "fow\\fow_CNW.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW CSE", "fow\\fow_CSE.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW CSW", "fow\\fow_CSW.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW EEE", "fow\\fow_EEE.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW JNE", "fow\\fow_JNE.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW JNW", "fow\\fow_JNW.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW JSE", "fow\\fow_JSE.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW JSW", "fow\\fow_JSW.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW NNN", "fow\\fow_NNN.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW SSS", "fow\\fow_SSS.png");
	MaterialManager::Instance()->AddDiffuse(d3d, base_path, "FOW WWW", "fow\\fow_WWW.png");
}
//===============================================================================================================================