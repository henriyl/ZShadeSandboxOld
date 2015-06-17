#include "CreateGameDirectory.h"
#include "BetterString.h"
#include "CGlobal.h"
//================================================================================================================
//================================================================================================================
CreateGameDirectory::CreateGameDirectory(GameDirectory2D* gd, GameType2D gameType)
{
	_mkdir(gd->GAME_FOLDER.c_str());
	_mkdir(gd->m_required_textures.c_str());
	_mkdir(gd->m_sprites_path.c_str());
	_mkdir(gd->m_menu_sprites_path.c_str());
	_mkdir(gd->m_hud_path.c_str());
	_mkdir(gd->m_tiles_path.c_str());
	_mkdir(gd->m_scripts_path.c_str());
	_mkdir(gd->m_music_path.c_str());
	_mkdir(gd->m_animation_profiles_path.c_str());
	_mkdir(gd->m_animation_profile_sprites_path.c_str());
	_mkdir(gd->m_animation_profile_tiles_path.c_str());
	_mkdir(gd->m_xml_path.c_str());
	_mkdir(gd->m_xml_maps_path.c_str());
	_mkdir(gd->m_xml_sprites_path.c_str());
	_mkdir(gd->m_xml_menu_path.c_str());
	_mkdir(gd->m_xml_hud_path.c_str());
	_mkdir(gd->m_xml_worlds_path.c_str());

	if (gameType == TOPDOWN)
	{
		CFileWriter fileWriter;
		fileWriter.Open(gd->m_game_ini_path);
		fileWriter.Write("[GAME]\n");
		fileWriter.Write("\n");
		fileWriter.Write(";Game can either be Topdown or Platformer, 3D for 3D game\n");
		fileWriter.Write("GameType=Topdown\n");
		fileWriter.Close();
	}
	else if (gameType == PLATFORMER)
	{
		CFileWriter fileWriter;
		fileWriter.Open(gd->m_game_ini_path);
		fileWriter.Write("[GAME]\n");
		fileWriter.Write("\n");
		fileWriter.Write(";Game can either be Topdown or Platformer, 3D for 3D game\n");
		fileWriter.Write("GameType=Platformer\n");
		fileWriter.Close();
	}

	//Find important files to copy
	string texture_dir = "bin";
	
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL, myPath, _MAX_PATH);
	
	BetterString bs(myPath);
	bs = bs.Strip("Debug\\ZShadeSandbox.exe");

	//string required_textures_path = "\\RequiredTextures";//string( texture_dir ).append("RequiredTextures");
	BetterString path = bs + "\\bin\\RequiredTextures";

	string required_textures_path = string(path.CStringBuffer());

	string new_required_textures_path = gd->m_required_textures;
	
	char* in_path = new char[strlen(required_textures_path.c_str()) + 1];
	//memset(in_path, 0, strlen(required_textures_path.c_str()) + 1);
	//in_path[strlen(required_textures_path.c_str())] = '\\';
	strcpy_s(in_path, strlen(required_textures_path.c_str()) + 1, required_textures_path.c_str());
	char* out_path = new char[strlen(new_required_textures_path.c_str()) + 1];
	//memset(out_path, 0, strlen(new_required_textures_path.c_str()) + 1);
	//out_path[strlen(new_required_textures_path.c_str())] = '\\';
	strcpy_s(out_path, strlen(new_required_textures_path.c_str()) + 1, new_required_textures_path.c_str());

	CopyAllFiles(in_path, out_path, "");
}
//================================================================================================================
CreateGameDirectory::CreateGameDirectory(GameDirectory3D* gd)
{
	_mkdir(gd->GAME_FOLDER.c_str());
	_mkdir(gd->m_required_textures.c_str());
	_mkdir(gd->m_menu_sprites_path.c_str());
	_mkdir(gd->m_hud_path.c_str());
	_mkdir(gd->m_scripts_path.c_str());
	_mkdir(gd->m_music_path.c_str());
	_mkdir(gd->m_materials_path.c_str());
	_mkdir(gd->m_heightmaps_path.c_str());
	_mkdir(gd->m_models_path.c_str());
	_mkdir(gd->m_textures_path.c_str());
	_mkdir(gd->m_xml_path.c_str());
	_mkdir(gd->m_xml_menu_path.c_str());
	_mkdir(gd->m_xml_hud_path.c_str());
	_mkdir(gd->m_xml_maps_path.c_str());
	
	CFileWriter fileWriter;
	fileWriter.Open(gd->m_game_ini_path);
	fileWriter.Write("[GAME]\n");
	fileWriter.Write("\n");
	fileWriter.Close();
	
	//Find important files to copy
	string texture_dir = "bin";
	
	char myPath[_MAX_PATH+1];
	GetModuleFileName(NULL, myPath, _MAX_PATH);
	
	BetterString bs(myPath);
	bs = bs.Strip("Debug\\ZShadeSandbox.exe");

	//string required_textures_path = "\\RequiredTextures";//string( texture_dir ).append("RequiredTextures");
	BetterString path = bs + "\\bin\\RequiredTextures";

	string required_textures_path = string(path.CStringBuffer());
	string new_required_textures_path = gd->m_required_textures;
	
	char* in_path = new char[strlen(required_textures_path.c_str()) + 1];
	strcpy_s(in_path, strlen(required_textures_path.c_str()) + 1, required_textures_path.c_str());
	char* out_path = new char[strlen(new_required_textures_path.c_str()) + 1];
	strcpy_s(out_path, strlen(new_required_textures_path.c_str()) + 1, new_required_textures_path.c_str());

	CopyAllFiles(in_path, out_path, "");
}
//================================================================================================================
void CreateGameDirectory::CopyAllFiles(char path_in[MAX_PATH], char path_out[MAX_PATH], char sub_path[MAX_PATH])
{
	BetterString strIn(path_in);
	strIn += "\\";

	BetterString strOut(path_out);
	strOut += "\\";

	strcpy_s(path_in, strIn.size(), strIn.toString().c_str());
	strcpy_s(path_out, strOut.size(), strOut.toString().c_str());

	//if (path_in[strlen(path_in) - 1] != '\\')
	//	strcat_s(path_in, strlen(path_in) - 1, "\\");

	//if (path_out[strlen(path_out) -1] != '\\')
	//	strcat_s(path_out, strlen(path_out) - 1, "\\");


	//varibles used for the searching
	HANDLE hFind;
	WIN32_FIND_DATA dataFind;
	bool bMoreFiles = true;

	//strings
	char findpath[250];
	char fullsrcpath[250];
	char fulldestpath[250];
	char in_temp[250];
	char out_temp[250];

	//tell it to find *.*
	sprintf_s(fullsrcpath, "%s%s*.*", path_in, sub_path);
	hFind = FindFirstFile(fullsrcpath, &dataFind);

	while (hFind != INVALID_HANDLE_VALUE && bMoreFiles == true)
	{
		sprintf_s(in_temp, "%s%s%s", path_in, sub_path, dataFind.cFileName);
		sprintf_s(out_temp, "%s%s%s", path_out, sub_path, dataFind.cFileName);

		CopyFile(in_temp, out_temp, TRUE);
		bMoreFiles = FindNextFile(hFind, &dataFind);
	}

	//find new directory
	sprintf_s(fullsrcpath, "%s%s*.", path_in, sub_path);
	hFind = FindFirstFile(fullsrcpath, &dataFind);
	bMoreFiles = true;

	while (hFind != INVALID_HANDLE_VALUE && bMoreFiles == true)
	{
		//make sure it's directory
		if ((dataFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 && dataFind.cFileName[0] != '.')
		{
			char temp[50];

			sprintf_s(temp, "%s\\", dataFind.cFileName);

			sprintf_s(in_temp, "%s%s%s", path_in, sub_path, temp);
			sprintf_s(out_temp, "%s%s%s", path_out, sub_path, temp);
			sprintf_s(fullsrcpath, "%s%s", path_in, sub_path);
			sprintf_s(fulldestpath, "%s%s", path_out, sub_path);

			CreateDirectory(out_temp, NULL);
			CopyAllFiles(fullsrcpath, fulldestpath, temp);
		}
		bMoreFiles = FindNextFile(hFind, &dataFind);
	}
}
//================================================================================================================