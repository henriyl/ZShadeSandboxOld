#include "EnvironmentHUDHelper.h"
#include "Convert.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "Environment2D.h"
#include "EnvironmentFactory.h"
//================================================================================================================
//================================================================================================================
EnvironmentHUDHelper::EnvironmentHUDHelper()
{
}
//================================================================================================================
EnvironmentHUDHelper::~EnvironmentHUDHelper()
{
}
//================================================================================================================
void EnvironmentHUDHelper::AddHUDImage(ZShadeSandboxGraphics::Image* image)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetHUDSystem()->AddHUDImage(image);
}
//================================================================================================================
void EnvironmentHUDHelper::AddHUDText(ZShadeSandboxGraphics::Text* text)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetHUDSystem()->AddHUDText(text);
}
//================================================================================================================
void EnvironmentHUDHelper::CreateHUD(string hud_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	HUD* hud = new HUD(env2D->GetD3DSystem());
	//hud->SetHUDName(hud_name);
	env2D->GetHUDSystem()->AddHUD(hud);
}
//================================================================================================================
//void EnvironmentHUDHelper::GrabHUDNames(vector<string>& names)
//{
//	Environment2D::Instance()->GetHUDSystem()->GetHUDNames(names);
//}
//================================================================================================================
void EnvironmentHUDHelper::LoadHUD(string hud_filename, string& hud_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	ZShadeXMLLoader::LoadHUDXML(hud_filename,
		env2D->GetGD2D(), env2D->GetEngineOptions(), env2D->GetD3DSystem(),
		0, env2D->GetHUDSystem());
}
//================================================================================================================
void EnvironmentHUDHelper::SaveHUD(string hud_filename)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	ZShadeXMLSaver::SaveHUDXML(hud_filename, env2D->GetHUDSystem());
}
//================================================================================================================