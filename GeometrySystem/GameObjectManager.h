
#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include <vector>

class GameObject;
class LevelParser;
class LightsManager;

class GameObjectManager
{
public:
	GameObjectManager();
	virtual ~GameObjectManager();

	// Initialize
	bool		InitGameObjectManager(const std::string& lvlFile);
	bool		InitGameObjects();

	// Update
	void		UpdateEverything(float dt);
	void		UpdateByID(int id, float dt);
	void		UpdateByType(int type, float dt);
	void		UpdateByName(const std::string& name, float dt);
	void		UpdateByVisibility(bool flag, float dt);

	// Render
	void		RenderEverything();
	void		RenderByID(int id);
	void		RenderByType(int type);
	void		RenderByName(const std::string& name);
	void		RenderByVisibility(bool flag);

	// Lights
	void		GenerateLightList();

	// Getter
	GameObject*	GetObjectByType(int type);
	GameObject*	GetObjectByID(int id);
	GameObject*	GetObjectByName(const std::string& name);
	std::string GetEnvironmentMapName();

	// Kill me!
	void		Kill();

private:
	std::vector<GameObject*>	m_vecGameObjList;
	LevelParser*				m_pLvL;
};

#endif