#include "EnvironmentFactory.h"
//==================================================================================================================================
//==================================================================================================================================
EnvironmentFactory* EnvironmentFactory::instance = 0;
//==================================================================================================================================
EnvironmentFactory::EnvironmentFactory()
:   m_EngineOptions(0)
{
}
//==================================================================================================================================
Environment* EnvironmentFactory::SelectEnvironment(string name)
{
	map<string, Environment*>::iterator i = mEnvironmentList.find(name);
	
	if (i != mEnvironmentList.end())
		return (*i).second;
	
	return NULL;
}
//==================================================================================================================================
Environment* EnvironmentFactory::CreateEnvironment(string name, EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua)
{
	// When an environment is created assign the engine options so we can have a standard copy
	if (m_EngineOptions == 0)
		m_EngineOptions = engineOptions;
	
	Environment* env = new Environment(engineOptions, base_window_name, render_window_name, init_lua);
	
	mEnvironmentList.insert( make_pair(name, env) );

	return env;
}
//==================================================================================================================================
void EnvironmentFactory::DestroyEnvironment(string name)
{
	bool found = false;
	map<string, Environment*>::iterator iter;
	map<string, Environment*>::iterator i = mEnvironmentList.find(name);
	
	if (i != mEnvironmentList.end())
	{
		iter = i;
		found = true;
	}
	
	if (found)
	{
		mEnvironmentList.erase(iter);
	}
}
//==================================================================================================================================