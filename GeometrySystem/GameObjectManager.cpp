
#include "GameObjectManager.h"
#include "GameObject.h"
#include "../LightingSystem/LightsManager.h"
#include "../Parser/LevelParser.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"

//----------------------------------------------------------------------------------------
// Purpose  : Constructor
//----------------------------------------------------------------------------------------
GameObjectManager::GameObjectManager()
{
	m_pLvL		=	new LevelParser();
	m_vecGameObjList.clear();
}

//----------------------------------------------------------------------------------------
// Purpose  : Destructor
//----------------------------------------------------------------------------------------
GameObjectManager::~GameObjectManager()
{
	Kill();
}

//----------------------------------------------------------------------------------------
// Purpose  : Kill GameObjectManager contents
//----------------------------------------------------------------------------------------
void	GameObjectManager::Kill()
{
	SAFE_DELETE(m_pLvL);

	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		SAFE_DELETE((*iter));
	}

	m_vecGameObjList.clear();
}

//----------------------------------------------------------------------------------------
// Purpose  : Initialize GameObjectManager contents
//----------------------------------------------------------------------------------------
bool	GameObjectManager::InitGameObjectManager(const std::string& lvlFile)
{
	// Parse LVL file & fill up GameObject's list
	if(!m_pLvL->ParseLVL(lvlFile, m_vecGameObjList))
	{
		LogManager::GetInstance()->WriteToFile("GameObject's List Initialization FAILED");
		return false;
	}
	else
	{
		LogManager::GetInstance()->WriteToFile("GameObject's List Initialized");
	
		if(!InitGameObjects())
		{
			LogManager::GetInstance()->WriteToFile("InitGameObjectManager() FAILED");
			return false;
		}

		// Generate List of Light Objects from GameObjects!
		GenerateLightList();
	}	

	return true;
}

//----------------------------------------------------------------------------------------
// Purpose  : Initialize & Load each GameObject
//----------------------------------------------------------------------------------------
bool	GameObjectManager::InitGameObjects()
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		(*iter)->Init();
	}

	return true;
}

//----------------------------------------------------------------------------------------
// Purpose  : Get Game Object by ID
//----------------------------------------------------------------------------------------
GameObject*	GameObjectManager::GetObjectByID(int id)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectID() == id)
			return (*iter);
	}
}

GameObject* GameObjectManager::GetObjectByType( int type )
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectType() == type)
			return (*iter);
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Get Game Object by Name
//----------------------------------------------------------------------------------------
GameObject*	GameObjectManager::GetObjectByName(const std::string& name)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if (!((*iter)->getObjectName().compare(name.c_str())))
			return (*iter);
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Get Game Object by Name
//----------------------------------------------------------------------------------------
std::string GameObjectManager::GetEnvironmentMapName()
{
	// Simply return Environment Map of any object, here first!
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();
	return (*iter)->getEnvMap();
}


//----------------------------------------------------------------------------------------
// Purpose  : Update Every GameObject
//----------------------------------------------------------------------------------------
void	GameObjectManager::UpdateEverything(float dt)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		(*iter)->Update(dt);
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Update GameObject by ID
//----------------------------------------------------------------------------------------
void	GameObjectManager::UpdateByID(int id, float dt)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectID() == id)
		{
			(*iter)->Update(dt);
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render GameObject by type
//----------------------------------------------------------------------------------------
void GameObjectManager::UpdateByType( int type, float dt )
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectType() == type)
		{
			(*iter)->Update(dt);
		}
	}
}
	
//----------------------------------------------------------------------------------------
// Purpose  : Update GameObject by Name
//----------------------------------------------------------------------------------------
void	GameObjectManager::UpdateByName(const std::string& name, float dt)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if(!((*iter)->getObjectName().compare(name)))
		{
			(*iter)->Update(dt);
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Update GameObject by visibility check
//----------------------------------------------------------------------------------------
void	GameObjectManager::UpdateByVisibility(bool flag, float dt)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->IsModelVisible())
		{
			(*iter)->Update(dt);
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render Every GameObject
//----------------------------------------------------------------------------------------
void	GameObjectManager::RenderEverything()
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		(*iter)->Render();
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render GameObject by ID
//----------------------------------------------------------------------------------------
void	GameObjectManager::RenderByID(int id)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectID() == id)
		{
			(*iter)->Render();
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render GameObject by type
//----------------------------------------------------------------------------------------
void GameObjectManager::RenderByType( int type )
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->getObjectType() == type)
		{
			(*iter)->Render();
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render GameObject by Name
//----------------------------------------------------------------------------------------
void	GameObjectManager::RenderByName(const std::string& name)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if(!((*iter)->getObjectName().compare(name)))
		{
			(*iter)->Render();
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Render GameObject by visibility check
//----------------------------------------------------------------------------------------
void	GameObjectManager::RenderByVisibility(bool flag)
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		if ((*iter)->IsModelVisible())
		{
			(*iter)->Render();
		}
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : Generate list of lights from all the GameObjects
//----------------------------------------------------------------------------------------
void GameObjectManager::GenerateLightList()
{
	std::vector<GameObject*>::iterator iter = m_vecGameObjList.begin();

	for ( ; iter != m_vecGameObjList.end() ; iter++)
	{
		int type = (*iter)->getObjectType();
		switch(type)
		{
		case POINT_LIGHT:
			{
				LightsManager::GetInstance()->GatherPointLightObjects((*iter));
			}
			break;

		case DIRECTIONAL_LIGHT:
			{
				LightsManager::GetInstance()->GatherDirectionalLightObjects((*iter));
			}
			break;

		case SPOT_LIGHT:
			break;

		default:
			break;
		}
	}
}






