
#ifndef LIGHTS_MANAGER_H
#define LIGHTS_MANAGER_H

#include <vector>

class GameObject;
class PointLightObject;
class DirectionalLightObject;

class LightsManager
{
public:
	static LightsManager* GetInstance()
	{
		static LightsManager inst;
		return &inst;
	}

	virtual ~LightsManager();

	void					GatherPointLightObjects(GameObject* obj);
	void					GatherDirectionalLightObjects(GameObject* obj);

	// Getter
	inline int				GetPointLightCount(){ return m_iPointNumLights; }
	inline int				GetDirectionalLightCount(){ return m_iDirectionalNumLights; }

	PointLightObject*		GetPointLight(int id);
	DirectionalLightObject* GetDirectionalLight(int id);

private:
	LightsManager();

	LightsManager(const LightsManager&);
	LightsManager operator=(const LightsManager&);

	int										m_iPointNumLights;
	int										m_iDirectionalNumLights;
	std::vector<PointLightObject*>			m_vecPointLightObjects;
	std::vector<DirectionalLightObject*>	m_vecDirectionalLightObjects;	
}
;

#endif
