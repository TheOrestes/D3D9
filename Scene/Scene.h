// ***************************************************************
//  Scene     
//  -------------------------------------------------------------
//  Purpose : Load our Game Scene Here!
// ***************************************************************

#ifndef Scene_H
#define Scene_H

#include <Windows.h>
#include <d3dx9.h>
#include <string>
#include <vector>

class Skybox;
class GameObjectManager;
class StaticGeometryManager;
class StaticGeometry;
class SceneLight;
class BruteForceTerrain;
class TextParser;
class ParticleFactory;
class ParticleSystem;
class Grass;
class Skybox;
class Screenshot;
class Clock;

//#define X_MESH
//#define OBJ_MESH

#if defined X_MESH
	class StaticGeometryManager;
#elif defined OBJ_MESH
	class ObjMeshLoader;
#endif

class Scene	 
{
public:
	virtual ~Scene();

	// Singleton Instance
	static Scene* GetInstance()
	{
		static Scene inst;
		return &inst;
	}

	bool	Initialize(Clock* timer);
	bool	PrepareTerrain();
	bool	PrepareSky();
#if defined X_MESH
	bool	PreCacheGeometry(const std::string& name);
#endif
	void	Render();
	void	Update(float dt);
	void	Shutdown();

	// Give away the sky object
	inline Skybox*	getSkyBoxObject(){ return m_pSKY; }

	//Skybox*				sky;	
	TextParser*						m_pTextParser;
	
#if defined X_MESH
	StaticGeometryManager*			staGeomMgr;
#elif defined OBJ_MESH
	ObjMeshLoader*					m_pObjMesh;
#endif

	GameObjectManager*				m_pGameObjectMgr;
	
	BruteForceTerrain*				m_pTerrain;

	D3DXVECTOR4						m_GlobalLightDirection;

	ParticleFactory*				m_ParticleFactory;
	std::vector<ParticleSystem*>	m_VecPSystem;

	Grass*							m_pGrass;
	Skybox*							m_pSKY;
	Screenshot*						m_pScreenGrabber;
	Clock*							m_pTimer;

	//EffectExplosion*		m_pExplosion;	

private:
	Scene(const Scene&);				// Explicitly disallow the use of 
	Scene operator=(const Scene&);		// compiler-generated functions 

	Scene();
};

#endif