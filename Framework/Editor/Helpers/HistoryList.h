//******************************************************************************************************
// HistoryList.h
//******************************************************************************************************
#ifndef __HISTORYLIST_H
#define __HISTORYLIST_H
/*
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <vector>
#include <map>
#include "StackMap.h"
#include "Sprite.h"
#include "Tile.h"
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
struct TILE_HISTORY
{
	Tile* tile;
	string revertImageName;
};
//-----------------------------------------------------------------------------------------------------------------
//This will be used in the undo/redo actions
class HistoryList
{
public:
	HistoryList();
	HistoryList(const HistoryList& o);
	~HistoryList();

	void AddSpritesToUndoList(string action, vector<Sprite*> sprites);
	void UndoSpriteAction(string& action, vector<Sprite*>& sprites);
	void RedoSpriteAction(string& action, vector<Sprite*>& sprites);

	void AddTilesToUndoList(string action, vector<TILE_HISTORY*> tiles);
	void UndoTileAction(string& action, vector<TILE_HISTORY*>& tiles);
	void RedoTileAction(string& action, vector<TILE_HISTORY*>& tiles);

	void ClearSpriteUndoRedo();
	void ClearTileUndoRedo();

	bool IsSpriteUndoListEmpty();
	bool IsSpriteRedoListEmpty();

	bool IsTileUndoListEmpty();
	bool IsTileRedoListEmpty();

	bool IsSpriteUndoListFull();
	bool IsSpriteRedoListFull();

	bool IsTileUndoListFull();
	bool IsTileRedoListFull();

	void ClearSpriteUndo();
	void ClearSpriteRedo();

	void ClearTileUndo();
	void ClearTileRedo();

private:
	StackMap<string, vector<Sprite*>> m_undo_sprite_list;
	StackMap<string, vector<Sprite*>> m_redo_sprite_list;

	StackMap<string, vector<TILE_HISTORY*>> m_undo_tile_list;
	StackMap<string, vector<TILE_HISTORY*>> m_redo_tile_list;

//	void AddActionSprite(string action, vector<Sprite*> sprite);
//	void AddActionTile(string action, vector<Tile*> tiles);
//
//	//void AddSpriteFromMoveUndo(Sprite* sprite);
//	//void AddSpriteFromMoveRedo(Sprite* sprite);
//	//void UndoSpriteFromMove(Sprite*& sprite);
//	//void RedoSpriteFromMove(Sprite*& sprite);
//
//	void UndoSpriteAction(string& action, vector<Sprite*>& sprite);
//	void RedoSpriteAction(string& action, vector<Sprite*>& sprite);
//
//	void UndoTileAction(string& action, vector<Tile*>& tiles);
//	void RedoTileAction(string& action, vector<Tile*>& tiles);
//
//
//	int GetUndoSpriteIndex() { return m_undo_sprite_index; }
//	int GetRedoSpriteIndex() { return m_redo_sprite_index; }
//
//	int GetUndoTileIndex() { return m_undo_tile_index; }
//	int GetRedoTileIndex() { return m_redo_tile_index; }
//
//private:
//	//vector<Sprite*> for the undo/redo sprite list
//	//undo: 1st sprite is the sprite that was moved and 2nd sprite is the new moved sprite
//	//redo: 1st sprite is the new moved sprite and 2nd sprite is the sprite that was moved
//	//if no move then only 1st slot is used
//
//	map<string, vector<Sprite*>> m_history_sprite_actions;
//	map<string, vector<Tile*>> m_history_tile_actions;
//	
//	vector<Sprite*> m_moved_sprites_undo;
//	vector<Sprite*> m_moved_sprites_redo;
//
//	map<string, vector<Sprite*>> m_redo_sprite_list;
//	map<string, vector<Tile*>> m_redo_tile_list;
//
//	int m_undo_sprite_index;
//	int m_redo_sprite_index;
//
//	int m_undo_tile_index;
//	int m_redo_tile_index;
//
//	int MAX_UNDO_ACTIONS;
};
*/
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__HISTORYLIST_H