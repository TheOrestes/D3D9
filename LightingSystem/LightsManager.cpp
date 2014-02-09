
#include "LightsManager.h"
#include "../GeometrySystem/GameObject.h"
#include "../LightingSystem/PointLightObject.h"
#include "../LightingSystem/DirectionalLightObject.h"
#include "../Misc/Helper.h"
#include "../Misc/LogManager.h"

/////////////////////////////////////////////////////////////////////////////////////////
LightsManager::LightsManager()
{
	m_iPointNumLights = 0;
	m_vecPointLightObjects.empty();

	m_iDirectionalNumLights = 0;
	m_vecDirectionalLightObjects.empty();
}

/////////////////////////////////////////////////////////////////////////////////////////
LightsManager::~LightsManager()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
void	LightsManager::GatherPointLightObjects(GameObject* obj)
{
	LogManager::GetInstance()->WriteToFile("Generating Point Light list...");
	LogManager::GetInstance()->WriteToFile(obj->getObjectName().c_str(), "added...");

	m_vecPointLightObjects.push_back(static_cast<PointLightObject*>(obj));
	m_iPointNumLights = m_vecPointLightObjects.size();
}

/////////////////////////////////////////////////////////////////////////////////////////
PointLightObject* LightsManager::GetPointLight( int id )
{
	return	m_vecPointLightObjects.at(id);
}

//////////////////////////////////////////////////////////////////////////
void	LightsManager::GatherDirectionalLightObjects(GameObject* obj)
{
	LogManager::GetInstance()->WriteToFile("Generating Directional Light list...");
	LogManager::GetInstance()->WriteToFile(obj->getObjectName().c_str(), "added...");

	m_vecDirectionalLightObjects.push_back(static_cast<DirectionalLightObject*>(obj));
	m_iDirectionalNumLights = m_vecDirectionalLightObjects.size();
}

DirectionalLightObject* LightsManager::GetDirectionalLight( int id )
{
	return	m_vecDirectionalLightObjects.at(id);
}

