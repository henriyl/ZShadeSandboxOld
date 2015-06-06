//==================================================================================================================================
// EnvironmentFactory.h
//
//==================================================================================================================================
// History
//
// -Created on 3/12/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __ENVIRONMENTFACTORY_H
#define __ENVIRONMENTFACTORY_H
//==================================================================================================================================
//==================================================================================================================================
#include <map>
#include "Environment.h"
using namespace std;
//==================================================================================================================================
class EnvironmentFactory
{
	static EnvironmentFactory* instance;
	
public:
	
	static EnvironmentFactory* Instance()
	{
		if (instance == 0)
			instance = new EnvironmentFactory();
		return instance;
	}
	
public:
	
	EnvironmentFactory();
	
	EngineOptions* GetEngineOptions() { return m_EngineOptions; }
	Environment* SelectEnvironment(string name);
	Environment* CreateEnvironment(string name, EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua);
	void DestroyEnvironment(string name);
	
	string& ActiveEnvironmentName() { return m_ActiveEnvironmentName; }
	string ActiveEnvironmentName() const { return m_ActiveEnvironmentName; }
	
private:
	
	string m_ActiveEnvironmentName;
	EngineOptions* m_EngineOptions;
	map<string, Environment*> mEnvironmentList;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__ENVIRONMENTFACTORY_H