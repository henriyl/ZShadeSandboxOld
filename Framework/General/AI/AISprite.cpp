#include "AISprite.h"
//#include "Commands.h"
#include "Scripting.h"
//-----------------------------------------------------------------------------------------------
AISprite::AISprite(D3D* d3d)
:	Sprite(d3d),
	m_run_behavior(false),
	m_run_behavior_default(false),
	//m_script_path(""),
	//m_script_name(""),
	m_default_behavior(""),
	m_behavior_name(""),
	tree(0),
	m_TargetSprite(0)
{
}
//-----------------------------------------------------------------------------------------------
AISprite::AISprite(const AISprite& o) : Sprite(o)
{
}
//-----------------------------------------------------------------------------------------------
AISprite::~AISprite()
{
}
//-----------------------------------------------------------------------------------------------
void AISprite::SetRunBehavior(bool set)
{
	m_run_behavior = set;
}
//-----------------------------------------------------------------------------------------------
void AISprite::SetRunBehaviorDefault(bool set)
{
	m_run_behavior_default = set;
}
//-----------------------------------------------------------------------------------------------
void AISprite::SetBehaviorName(string name)
{
	m_behavior_name = name;
}
//-----------------------------------------------------------------------------------------------
void AISprite::updateBehavior()
{
	if (tree == 0) return;
	
	//Think about what we are doing
	Think();
	
	if (m_run_behavior)
	{
		tree->RunBehavior(m_behavior_name);
		tree->TickActiveBehavior();
	}
	else
	{
		if (m_run_behavior_default)
		{
			//Run default behavior
			tree->RunBehavior(m_default_behavior);
			tree->TickActiveBehavior();
		}
	}
}
//-----------------------------------------------------------------------------------------------
void AISprite::AddDefaultBehavior(string p_default_behavior_name)
{
	m_default_behavior = p_default_behavior_name;
}
//-----------------------------------------------------------------------------------------------
void AISprite::AddBehaviorSequence(string p_behavior_name, string p_sequence_name, string p_action_func_name, string p_condition_func_name)
{
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		if (m_behaviors[i]->name == p_behavior_name)
		{
			SEQUENCE_INFO* si = new SEQUENCE_INFO();
			si->name = p_sequence_name;
			si->action_func_name = p_action_func_name;
			si->condition_func_name = p_condition_func_name;
			m_behaviors[i]->sequence_info.push_back( si );
		}
	}
}
//-----------------------------------------------------------------------------------------------
void AISprite::RegisterBehavior(string p_behavior_name)
{
	BEHAVIOR_INFO* bi = new BEHAVIOR_INFO();

	bi->name = p_behavior_name;

	m_behaviors.push_back( bi );
}
//-----------------------------------------------------------------------------------------------
void AISprite::CreateTree()
{
	tree = new BehaviorTree();

	for (int i = 0; i < m_behaviors.size(); i++)
	{
		Behavior* aiBehavior = new Behavior(m_behaviors[i]->name);
		{
			PrioritySelector* selector = new PrioritySelector();
			{
				for (int j = 0; j < m_behaviors[i]->sequence_info.size(); j++)
				{
					Sequence* sequence = new Sequence();
					{
						Node* sequence_node = new TaskNode();
						{
							sequence_node->SetSequenceLevel(1);
							Decorator* sequence_decorator = new Decorator();
							{
								LuaTask* action = new LuaTask();
								LuaDelegate* condition = new LuaDelegate(this);
								action->SetLuaState(Scripting::LuaState());
								condition->SetLuaState(Scripting::LuaState());
								string path = Scripting::GetGameDirectory()->m_scripts_path;
								path.append("\\");
								path.append(m_script_name);
								action->AddLuaAction(this, m_behaviors[i]->sequence_info[j]->name, path, m_behaviors[i]->sequence_info[j]->action_func_name);
								condition->AddFunction(path, m_behaviors[i]->sequence_info[j]->condition_func_name);
								sequence_decorator->SetUseLua(true);
								sequence_decorator->AddDecoratedComponent(action);
								sequence_decorator->AddCondition(condition);
							}
							sequence_node->AddDecorator( sequence_decorator );
						}
						sequence->AddChildTask( sequence_node );
					}
					selector->AddSequence( sequence );
				}
			}
			aiBehavior->AddSelector( selector );
		}
		tree->InsertBehavior( aiBehavior );
	}
}
//-----------------------------------------------------------------------------------------------
void AISprite::Main()
{
	m_script->Main(1, this);
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "main");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::MoveScript()
{
	// I can move
	if (m_moving)
	{
		m_script->Move(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "move");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Think()
{
	// I can think about what to do
	if (m_thinking)
	{
		m_script->Think(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = m_script_path;//gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "think");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Talk()
{
	// I decided to talk
	if (m_talking)
	{
		m_script->Talk(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "talk");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Die()
{
	// I was hurt badly and am dying
	if (m_dying)
	{
		m_script->Die(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "die");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Hit()
{
	// I was hit
	if (m_hit)
	{
		m_script->Hit(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "hit");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Touch()
{
	// I was touched
	if (m_touched)
	{
		m_script->Touch(1, this);
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "touch");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	lua_pcall(l, 1, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Attack()
{
	// I will be attacking a sprite
	if (m_attacked)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Attack(2, this, m_TargetSprite);
		}
	}
	
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "attack");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	luaW_push(l, other_sprite);
	if (!Commands::GetInstance()->IsInMap(other_sprite, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), other_sprite);
	}
	lua_pcall(l, 2, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------
void AISprite::Target()
{
	// I will be targeting a sprite
	if (m_targeted)
	{
		if (m_TargetSprite != NULL)
		{
			m_script->Target(2, this, m_TargetSprite);
		}
	}
	/*if (m_script == "NONE") return;
	string sc = gd->m_scripts_path;
	sc.append("\\");
	sc.append(m_script);
	luaL_dofile(l, sc.c_str());
	lua_getglobal(l, "target");
	luaW_push(l, this);
	int id;
	if (!Commands::GetInstance()->IsInMap(this, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), this);
	}
	luaW_push(l, target_sprite);
	if (!Commands::GetInstance()->IsInMap(target_sprite, id))
	{
		Commands::GetInstance()->SetCurrentID( Commands::GetInstance()->GetCurrentID() + 1 );
		Commands::GetInstance()->SetSpriteToCreateTex(Commands::GetInstance()->GetCurrentID(), target_sprite);
	}
	lua_pcall(l, 2, 0, 0);*/
}
//-----------------------------------------------------------------------------------------------------------------