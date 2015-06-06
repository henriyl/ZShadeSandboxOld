#include "EnvironmentMenuHelper.h"
#include "Convert.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment2D.h"
#include "EnvironmentFactory.h"
//================================================================================================================
//================================================================================================================
EnvironmentMenuHelper::EnvironmentMenuHelper()
{
}
//================================================================================================================
EnvironmentMenuHelper::~EnvironmentMenuHelper()
{
}
//================================================================================================================
void EnvironmentMenuHelper::AddMenuBackground(string activeMenuName, ZShadeSandboxGraphics::Image* image)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->AddMenuBackgroundSprite(activeMenuName, image);
}
//================================================================================================================
void EnvironmentMenuHelper::AddMenuButton(string activeMenuName, ZShadeSandboxGraphics::Button* button)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->AddMenuButton(activeMenuName, button);
}
//================================================================================================================
void EnvironmentMenuHelper::AddMenuText(string activeMenuName, ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->AddMenuText(activeMenuName, text);
}
//================================================================================================================
void EnvironmentMenuHelper::CreateMenu(string menu_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	ZShadeSandboxEnvironment::Menu* m = new ZShadeSandboxEnvironment::Menu();
	m->SetMenuName(menu_name);
	env2D->GetMenuSystem()->AddMenu(m);
}
//================================================================================================================
void EnvironmentMenuHelper::GrabMenuNames(vector<string>& names)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->GetMenuNames(names);
}
//================================================================================================================
void EnvironmentMenuHelper::LoadAMenu(string menu_filename, string& menuName)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	ZShadeXMLLoader::LoadMenuXML(menu_filename,
		env2D->GetGD2D(), env2D->GetEngineOptions(), env2D->GetD3DSystem(),
		0, env2D->GetMenuSystem());
}
//================================================================================================================
void EnvironmentMenuHelper::SaveMenu(string menu_filename)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	ZShadeXMLSaver::SaveMenuXML(menu_filename, env2D->GetMenuSystem());
}
//================================================================================================================
void EnvironmentMenuHelper::SetActiveRenderedMenu(string menu_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->SetMenuToRender(menu_name);
}
//================================================================================================================
void EnvironmentMenuHelper::RemoveMenu(string menu_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetMenuSystem()->RemoveMenu(menu_name);
}
//================================================================================================================