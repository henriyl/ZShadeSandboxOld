//===============================================================================================
//AISprite.h
//===============================================================================================
#ifndef __AISPRITE_H
#define __AISPRITE_H
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
#include <string>
#include <vector>
using namespace std;
#include "BehaviorTree.h"
#include "PrioritySelector.h"
#include "Sequence.h"
#include "LuaTask.h"
#include "TaskNode.h"
#include "LuaDelegate.h"
#include "Sprite.h"
//#include "Script.h"
using namespace LuaBehaviorTree;
class Script;
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
struct SEQUENCE_INFO
{
	string name;
	string condition_func_name;
	string action_func_name;
};
//-----------------------------------------------------------------------------------------------
struct BEHAVIOR_INFO
{
	string name;
	vector<SEQUENCE_INFO*> sequence_info;
};
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
class AISprite : public Sprite
{
public:
	AISprite(D3D* d3d);
	AISprite(const AISprite& o);
	~AISprite();

	void SetRunBehavior(bool set);
	void SetRunBehaviorDefault(bool set);
	void SetBehaviorName(string name);

	void updateBehavior();

	void AddDefaultBehavior(string p_default_behavior_name);
	void AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name);
	void RegisterBehavior(string p_behavior_name);

	void CreateTree();
	
	//void AddScript(Script* s) { m_script = s; }
	//void AddScriptPath(string sp) { m_script_path = sp; }
	//void SetLuaState(lua_State* l) { this->l = l; }
	
	void Main();
	void Think();
	
	void MoveScript();
	void Talk();
	void Die();
	void Hit();
	void Touch();
	void Attack();
	void Target();
	
	// Who am I targeting or attacking
	void SetTargetSprite(AISprite* ts) { m_TargetSprite = ts; }
	AISprite* GetTargetSprite() { return m_TargetSprite; }
	
private:
	//lua_State* l;
	//string m_script_path;
	//string m_script_name;
	string m_default_behavior;
	//string m_brain;
	vector<BEHAVIOR_INFO*> m_behaviors;
	bool m_run_behavior;
	bool m_run_behavior_default;
	string m_behavior_name;//Current behavior to update
	BehaviorTree* tree;
	AISprite* m_TargetSprite;
};
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
#endif//__AISPRITE_H