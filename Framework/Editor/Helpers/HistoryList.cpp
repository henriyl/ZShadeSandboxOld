#include "HistoryList.h"
#include "Convert.h"
/*
//-----------------------------------------------------------------------------------------------------------------
HistoryList::HistoryList()
{
	//m_undo_sprite_index = m_redo_sprite_index = 0;
	//m_undo_tile_index = m_redo_tile_index = 0;
	//MAX_UNDO_ACTIONS = 5000;
	//m_moved_sprites_undo.resize(MAX_UNDO_ACTIONS);
	//m_moved_sprites_redo.resize(MAX_UNDO_ACTIONS);
}
//-----------------------------------------------------------------------------------------------------------------
HistoryList::HistoryList(const HistoryList& o)
{
}
//-----------------------------------------------------------------------------------------------------------------
HistoryList::~HistoryList()
{
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearSpriteUndoRedo()
{
	m_undo_sprite_list.Clear();
	m_redo_sprite_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearTileUndoRedo()
{
	m_undo_tile_list.Clear();
	m_redo_tile_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::AddSpritesToUndoList(string action, vector<Sprite*> sprites)
{
	//If an entry exists then keep going till we find a new entry
	string action_undo = "";
	vector<Sprite*> sprites_undo;
	int i = m_undo_sprite_list.GetStackCount();
	m_undo_sprite_list.GetNodeAtIndex(action_undo, sprites_undo, i);
	while (action_undo != "")
	{
		//We need a new entry
		m_undo_sprite_list.GetNodeAtIndex(action_undo, sprites_undo, ++i);
		m_undo_sprite_list.IncIndex();
	}
	m_undo_sprite_list.push( action, sprites );
	m_redo_sprite_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::UndoSpriteAction(string& action, vector<Sprite*>& sprites)
{
	m_undo_sprite_list.pop( action, sprites );
	m_redo_sprite_list.push( action, sprites );
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::RedoSpriteAction(string& action, vector<Sprite*>& sprites)
{
	m_redo_sprite_list.pop( action, sprites );
	m_undo_sprite_list.push( action, sprites );
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::AddTilesToUndoList(string action, vector<TILE_HISTORY*> tiles)
{
	string action_undo = "";
	vector<TILE_HISTORY*> tiles_undo;
	int i = m_undo_tile_list.GetStackCount();
	m_undo_tile_list.GetNodeAtIndex(action_undo, tiles_undo, i);
	while (action_undo != "")
	{
		//We need a new entry
		m_undo_tile_list.GetNodeAtIndex(action_undo, tiles_undo, ++i);
		m_undo_tile_list.IncIndex();
	}
	m_undo_tile_list.push( action, tiles );
	m_redo_tile_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::UndoTileAction(string& action, vector<TILE_HISTORY*>& tiles)
{
	m_undo_tile_list.pop( action, tiles );
	m_redo_tile_list.push( action, tiles );
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::RedoTileAction(string& action, vector<TILE_HISTORY*>& tiles)
{
	m_redo_tile_list.pop( action, tiles );
	m_undo_tile_list.push( action, tiles );
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsSpriteUndoListEmpty()
{
	return m_undo_sprite_list.Empty();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsSpriteRedoListEmpty()
{
	return m_redo_sprite_list.Empty();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsTileUndoListEmpty()
{
	return m_undo_tile_list.Empty();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsTileRedoListEmpty()
{
	return m_redo_tile_list.Empty();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsSpriteUndoListFull()
{
	return m_undo_sprite_list.Full();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsSpriteRedoListFull()
{
	return m_redo_sprite_list.Full();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsTileUndoListFull()
{
	return m_undo_tile_list.Full();
}
//-----------------------------------------------------------------------------------------------------------------
bool HistoryList::IsTileRedoListFull()
{
	return m_redo_tile_list.Full();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearSpriteUndo()
{
	m_undo_sprite_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearSpriteRedo()
{
	m_redo_sprite_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearTileUndo()
{
	m_undo_tile_list.Clear();
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::ClearTileRedo()
{
	m_redo_tile_list.Clear();
}*/
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//void HistoryList::AddSpriteFromMoveUndo(Sprite* sprite)
//{
//	m_moved_sprites_undo[m_undo_sprite_index - 1] = sprite;
//}
////-----------------------------------------------------------------------------------------------------------------
//void HistoryList::AddSpriteFromMoveRedo(Sprite* sprite)
//{
//	m_moved_sprites_redo[m_redo_sprite_index - 1] = sprite;
//}
//-----------------------------------------------------------------------------------------------------------------
//void HistoryList::UndoSpriteFromMove(Sprite*& sprite)
//{
//	if (m_moved_sprites_undo.size() == 0) return;
//
//	sprite = m_moved_sprites_undo[m_undo_sprite_index];
//
//	//Remove the sprite from undo list
//	m_moved_sprites_undo.erase(m_moved_sprites_undo.begin() + m_undo_sprite_index);
//
//	//Add the sprite to the redo list
//	m_moved_sprites_redo[m_redo_sprite_index] = sprite;
//}
////-----------------------------------------------------------------------------------------------------------------
//void HistoryList::RedoSpriteFromMove(Sprite*& sprite)
//{
//	if (m_moved_sprites_redo.size() == 0) return;
//
//	sprite = m_moved_sprites_redo[m_redo_sprite_index];
//
//	//Remove the sprite from the redo list
//	m_moved_sprites_redo.erase(m_moved_sprites_redo.begin() + m_redo_sprite_index);
//
//	//Add the sprite to the undo list
//	m_moved_sprites_undo[m_undo_sprite_index] = sprite;
//}
//-----------------------------------------------------------------------------------------------------------------
/*void HistoryList::AddActionSprite(string action, vector<Sprite*> sprite)
{
	action.append("-");
	action.append(Convert::ConvertIntToString(m_undo_sprite_index));
	m_history_sprite_actions.insert(make_pair(action, sprite));
	m_undo_sprite_index++;
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::AddActionTile(string action, vector<Tile*> tiles)
{
	action.append("-");
	action.append(Convert::ConvertIntToString(m_undo_tile_index));
	m_history_tile_actions.insert(make_pair(action, tiles));
	m_undo_tile_index++;
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::UndoSpriteAction(string& action, vector<Sprite*>& sprite)
{
	if (m_undo_sprite_index >= MAX_UNDO_ACTIONS) return;

	//Get the action that was just added most recently and remove it from the undo list
	bool action_found = false;
	int i = 0;
	map<string, vector<Sprite*>>::iterator iter = m_history_sprite_actions.begin();
	for (; iter != m_history_sprite_actions.end(); iter++, i++)
	{
		if (fabs((float)m_undo_sprite_index - (float)m_history_sprite_actions.size()) == i)
		{
			action = (*iter).first;
			sprite = (*iter).second;
			action_found = true;
			break;
		}
	}

	if (action_found)
	{
		m_history_sprite_actions.erase(action);
		m_undo_sprite_index--;

		//Add this to the redo list just in case we want to redo
		m_redo_sprite_list.insert(make_pair(action, sprite));
		m_redo_sprite_index++;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::RedoSpriteAction(string& action, vector<Sprite*>& sprite)
{
	if (m_redo_sprite_index >= MAX_UNDO_ACTIONS) return;

	//Get the action that was just added most recently and remove it from the undo list
	bool action_found = false;
	int i = 0;
	map<string, vector<Sprite*>>::iterator iter = m_redo_sprite_list.begin();
	for (; iter != m_redo_sprite_list.end(); iter++, i++)
	{
		if (fabs((float)m_redo_sprite_index - 1) == i)
		{
			action = (*iter).first;
			sprite = (*iter).second;
			action_found = true;
			break;
		}
	}

	if (action_found)
	{
		m_redo_sprite_list.erase(action);
		m_redo_sprite_index--;

		//Add it back into the undo list
		m_history_sprite_actions.insert(make_pair(action, sprite));
		m_undo_sprite_index++;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::UndoTileAction(string& action, vector<Tile*>& tiles)
{
	if (m_undo_tile_index >= MAX_UNDO_ACTIONS) return;

	//Get the action that was just added most recently and remove it from the undo list
	bool action_found = false;
	int i = 0;
	map<string, vector<Tile*>>::iterator iter = m_history_tile_actions.begin();
	for (; iter != m_history_tile_actions.end(); iter++, i++)
	{
		if (m_undo_tile_index == i)
		{
			action = (*iter).first;
			tiles = (*iter).second;
			action_found = true;
			break;
		}
	}

	if (action_found)
	{
		m_history_tile_actions.erase(action);
		m_undo_tile_index--;

		//Add this to the redo list just in case we want to redo
		m_redo_tile_list.insert(make_pair(action, tiles));
		m_redo_tile_index++;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void HistoryList::RedoTileAction(string& action, vector<Tile*>& tiles)
{
	if (m_redo_tile_index >= MAX_UNDO_ACTIONS) return;

	//Get the action that was just added most recently and remove it from the undo list
	bool action_found = false;
	int i = 0;
	map<string, vector<Tile*>>::iterator iter = m_redo_tile_list.begin();
	for (; iter != m_redo_tile_list.end(); iter++, i++)
	{
		if (m_redo_tile_index == i)
		{
			action = (*iter).first;
			tiles = (*iter).second;
			action_found = true;
			break;
		}
	}

	if (action_found)
	{
		m_redo_tile_list.erase(action);
		m_redo_tile_index--;

		//Add it back into the undo list
		m_history_tile_actions.insert(make_pair(action, tiles));
		m_undo_tile_index++;
	}
}
//-----------------------------------------------------------------------------------------------------------------
*/