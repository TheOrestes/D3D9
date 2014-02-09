#include "Scene.h"
#include "../RenderDevice/RenderDevice.h"
#include "../InputSystem/InputDevice.h"
#include "../Misc/LogManager.h"
#include "../Renderables/Skybox.h"
#include "../Renderables/BruteForceTerrain.h"
#include "../ParticleSystem/ParticleType.h"
#include "../ParticleSystem/ParticleFactory.h"
#include "../ParticleSystem/FireRing.h"
#include "../ParticleSystem/Rain.h"
#include "../ParticleSystem/Sprinkler.h"
#include "../ParticleSystem/CampFire.h"
#include "../Foliage/Grass.h"
#include "../Game/Camera.h"
#include "../Misc/EngineStats.h"
#include "../Misc/Screenshot.h"
#include "../Parser/TextParser.h"
#include "../Parser/LevelParser.h"
#include "../Misc/Helper.h"
#include "../Engine/Clock.h"


#if defined X_MESH
#include "../Renderables/StaticGeometry.h"
#include "../GeometrySystem/StaticGeometryManager.h"
#elif  defined OBJ_MESH
#include "../GeometrySystem/ObjMeshLoader.h"
#endif

#include "../GeometrySystem/GameObjectManager.h"
#include "../GeometrySystem/GameObject.h"
#include "../LightingSystem/PointLightObject.h"

#include "../GeometrySystem/AssimpLoader.h"
#include "../GeometrySystem/Model.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Scene::Scene()
{
	//mainAmbientLight	=	new SceneLight();
	m_pTextParser		=	new	TextParser();

#if defined X_MESH
	staGeomMgr			=	StaticGeometryManager::GetInstance();
#elif defined OBJ_MESH
	m_pObjMesh			=	new ObjMeshLoader();
#endif

	m_pGameObjectMgr	=	new GameObjectManager();
	
	// TO CHECK - cellSpacing should be an ODD number for code to work!
	m_pTerrain			=	new BruteForceTerrain(256, 256, 20, 3.0f);

	m_GlobalLightDirection = D3DXVECTOR4(0.0f, 5.0f, 0.0f, 1.0f);
	
	m_pGrass			=	new Grass();
	m_pSKY				=	new Skybox();
	m_pScreenGrabber	=	new Screenshot();

	D3DXMATRIX psysScene;
	D3DXMatrixIdentity(&psysScene);

	m_ParticleFactory	= new ParticleFactory();
	m_VecPSystem.clear();
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Scene::~Scene()
{
	Shutdown();
}

//------------------------------------------------------------------------
// Purpose  : Initialize game Scene here...
//------------------------------------------------------------------------
bool Scene::Initialize(Clock* timer)
{
	LogManager::GetInstance()->WriteToFile("initializing Scene...");

	// Grab the timer
	m_pTimer = timer;
	
	// Initialize Terrain
	if(!PrepareTerrain())
		return false;
	
#if defined X_MESH
	if(!PreCacheGeometry("Data\\Data\\Scene.txt"))
		return false;
#elif defined OBJ_MESH
	m_pTimer->StartTimer();

	if(!m_pObjMesh->Init("Data\\StaticGeometry\\ObjModels\\Torus.obj"))
		return false;

	m_pTimer->StopTimer();
	double time = m_pTimer->getStopWatchTime();

	LogManager::GetInstance()->WriteToFile("OBJ mesh loading = ", time);
#endif

	m_pTimer->StartTimer();

	if(!m_pGameObjectMgr->InitGameObjectManager("Data\\Level\\Level1.scn"))
		return false;
	
	m_pTimer->StopTimer();
	double time = m_pTimer->getStopWatchTime();
	LogManager::GetInstance()->WriteToFile("Assimp mesh loading = ", time);
		
	// Initialize Camera
	Camera::GetInstance()->Initialize();

	// Initialize Skybox
	if(!PrepareSky())
		return false;

	// [7/28/2012 DoD]
	// Register Camera as Observer to receive Update when Input State changes!
	InputDevice::GetInstance()->registerObserver(Camera::GetInstance());

	// Register Screenshot class as an Observer for InputDevice
	InputDevice::GetInstance()->registerObserver(m_pScreenGrabber);

	// Initialize Particle System
	m_VecPSystem.push_back(m_ParticleFactory->MakeParticleSystem(PTYPE_CAMPFIRE)); 	// Initialize Main Ambient Scene Light
 	//mainAmbientLight->SwitchOnLight(true);
	
	LogManager::GetInstance()->WriteToFile("Scene Initialized...");
	return true;
}

//------------------------------------------------------------------------ 
// Purpose  : Prepare terrain related stuff
//------------------------------------------------------------------------
bool	Scene::PrepareTerrain()
{
	std::string name = "Data\\Terrain\\Good.raw";

	if (!m_pTerrain->ReadRAWFIle(name))
		return false;

	if(!m_pTerrain->InitTerrain())
		return false;

	// Initialize Grass
	if (!m_pGrass->InitializeGrass("", m_pTerrain))
		return false;

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Prepapre SKybox
//------------------------------------------------------------------------
bool Scene::PrepareSky()
{
	std::string SkyBoxTexture = m_pGameObjectMgr->GetEnvironmentMapName();

	// Initialize Skybox
	m_pSKY->InitializeSky( SkyBoxTexture, 10000.0f);
	
	return true;
}

//------------------------------------------------------------------------ 
// Purpose  : PreCache all static geometry files by reading a simple txt file
//------------------------------------------------------------------------
#if defined X_MESH
bool	Scene::PreCacheGeometry(const std::string& fileName)
{
	if(!m_pTextParser->ReadFile(fileName))
		return false;
	else
	{
		LogManager::GetInstance()->WriteToFile("Precaching of Static Geometry started...");

		std::map<std::string, Helper::StaticGeomData> tempData = m_pTextParser->GetParserData();
		std::map<std::string, Helper::StaticGeomData>::iterator iter = tempData.begin();

		for ( ; iter != tempData.end() ; iter++)
		{
			std::string tempName = (*iter).first;
			Helper::StaticGeomData tempData = (*iter).second;

			staGeomMgr->AddToStaticGeometryList(tempName, &tempData);
		}

		return true;
	}
}
#endif

//------------------------------------------------------------------------
// Purpose  : Update the game Scene
//------------------------------------------------------------------------
void Scene::Update(float dt)
{
	
// 
// 	D3DXVECTOR3 temp(0.0f, 0.0f, 0.0f + x); 
// 
// 	float height = m_pTerrain->getHeight(temp.x, temp.z);
// 	

	static float x = 0.0f;
	x += 0.75f*dt;

	//static float y = 0.0f;
	//y += 5.0f*dt;

 	D3DXVECTOR3 transDwarfEnv = D3DXVECTOR3(0.0f, 120.0f, 0.0f);
	D3DXVECTOR3 transDwarfSpec = transDwarfEnv + D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	D3DXVECTOR3 transDwarfDiff = transDwarfEnv + D3DXVECTOR3(100.0f, 0.0f, 0.0f);
	D3DXVECTOR3 axisOfRot = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
 	D3DXVECTOR3 axSc = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

	/*D3DXVECTOR3 lightRotVector1;
	D3DXVECTOR3 lightRotVector2;
	D3DXVECTOR3 lightRotVector3;
	static float phi = 0.0f;
	phi += D3DX_PI / 256.0f;
	static float theta = 0.0f;
	theta += D3DX_PI / 256.0f;

	lightRotVector1.x = 100.0f * sin(phi) * cos(theta);
	lightRotVector1.y = 500.0f * sin(phi) * sin(theta);
	lightRotVector1.z = 100.0f * cos(phi);

	lightRotVector2.x = 50.0f * sin(phi) * cos(theta);
	lightRotVector2.y = 500.0f * sin(phi) * sin(theta);
	lightRotVector2.z = 50.0f * cos(phi);

	lightRotVector3.x = 150.0f * sin(phi) * cos(theta);
	lightRotVector3.y = 600.0f * sin(phi) * sin(theta);
	lightRotVector3.z = 150.0f * cos(phi);*/

	static float y = 0.0f;
	y += (D3DX_PI / 128.0f);

	D3DXVECTOR3 transLightY = D3DXVECTOR3(0.0f, 2.0f*sin(y), 0.0f);
	D3DXVECTOR3 transLightX = D3DXVECTOR3(5.0f*sin(y), 0.0f, 0.0f);

#if defined X_MESH
	StaticGeometry *dwarf_diff, *dwarf_spec, *dwarf_env;

	if (staGeomMgr != NULL)
	{
		dwarf_diff = staGeomMgr->GetGeometryByName("DWARF_DIFFUSE");
		dwarf_spec = staGeomMgr->GetGeometryByName("DWARF_SPEC");	
		dwarf_env = staGeomMgr->GetGeometryByName("DWARF_ENV");

		D3DXVECTOR3 axisOfRot = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		dwarf_diff->Translate(&transDwarfDiff);
		dwarf_diff->Rotate(&axisOfRot, x);
		dwarf_diff->Scale(&axSc);

		dwarf_spec->Translate(&transDwarfSpec);
		dwarf_spec->Rotate(&axisOfRot, x);
		dwarf_spec->Scale(&axSc);

		dwarf_env->Translate(&transDwarfEnv);
		dwarf_env->Rotate(&axisOfRot, x);
		dwarf_env->Scale(&axSc);

		staGeomMgr->Update(dt);
	}
#elif defined OBJ_MESH
	m_pObjMesh->Translate(&transDwarfDiff);
	m_pObjMesh->Rotate(&axisOfRot, x);
	m_pObjMesh->Scale(&axSc);
	m_pObjMesh->Update(dt);
#endif
	
	if(m_pGameObjectMgr)
	{
		//m_pGameObjectMgr->GetObjectByName("Zake")->setModelRot(axisOfRot, x);
		//static_cast<PointLightObject*>(m_pGameObjectMgr->GetObjectByName("OmniLight1"))->setLightVelocity(lightRotVector1);
		//static_cast<PointLightObject*>(m_pGameObjectMgr->GetObjectByName("OmniLight2"))->setLightVelocity(lightRotVector2);
		//static_cast<PointLightObject*>(m_pGameObjectMgr->GetObjectByName("GreenOmni"))->setLightVelocity(transLightX);
		//static_cast<PointLightObject*>(m_pGameObjectMgr->GetObjectByName("RedOmni"))->setLightVelocity(transLightY);
		m_pGameObjectMgr->UpdateEverything(dt);
	}
	
	// Update Sky
	if (m_pSKY)
	{
		m_pSKY->Update(dt);
	}

	// Update Grass
	if (m_pGrass)
	{
		//m_pGrass->UpdateGrass(dt);
	}

	// Update Particle System
	std::vector<ParticleSystem*>::iterator iter = m_VecPSystem.begin();
	for (; iter != m_VecPSystem.end() ; iter++)
	{
		(*iter)->Update(dt);
	}
}

//------------------------------------------------------------------------
// Purpose  : Render the game Scene
//------------------------------------------------------------------------
void Scene::Render()
{
	if (m_pSKY)
	{
		m_pSKY->RenderSkybox();
	}

#if defined X_MESH
	if (staGeomMgr != NULL)
	{
		staGeomMgr->preRender();
		staGeomMgr->Render();
		staGeomMgr->postRender();
	}
#elif defined OBJ_MESH
	if (m_pObjMesh)
	{
		for (UINT iSubset = 0 ; iSubset<m_pObjMesh->GetNumMaterials() ; iSubset++)
		{
			m_pObjMesh->RenderSubset(iSubset);
		}
	}
#endif

	if(m_pGameObjectMgr)
	{
		m_pGameObjectMgr->RenderEverything();
	}
	
	if (m_pGrass)
	{
		//m_pGrass->RenderGrass();
	}

	// Render Terrain
	if (m_pTerrain != NULL)
	{
		m_pTerrain->preRender();
		m_pTerrain->Render();
		m_pTerrain->postRender();
	}

	// Render Particle System
	std::vector<ParticleSystem*>::iterator iter = m_VecPSystem.begin();
	for (; iter != m_VecPSystem.end() ; iter++)
	{
		(*iter)->Render();
	}
}

//------------------------------------------------------------------------
// Purpose  : Shutdown!
//------------------------------------------------------------------------
void Scene::Shutdown()
{	
	// Remove text parser
	SAFE_DELETE(m_pTextParser);

#if defined X_MESH
	if (staGeomMgr)
	{
		staGeomMgr->Kill();
		staGeomMgr = NULL;
	}
	SAFE_DELETE(staGeomMgr);
#elif defined OBJ_MESH
	if (m_pObjMesh)
	{
		m_pObjMesh->Kill();
		m_pObjMesh = NULL;
	}
	SAFE_DELETE(m_pObjMesh);
#endif

	SAFE_DELETE(m_pGameObjectMgr);

	// remove all the particle systems in the scene!
	std::vector<ParticleSystem*>::iterator iter = m_VecPSystem.begin();
	for (; iter != m_VecPSystem.end() ; iter++)
	{
		SAFE_DELETE (*iter);
		LogManager::GetInstance()->WriteToFile("Removing Particle...");
	}
	m_VecPSystem.clear();

	// remove terrain
	SAFE_DELETE(m_pTerrain);

	// remove grass	
	SAFE_DELETE(m_pTextParser);
	SAFE_DELETE(m_pTerrain);
	SAFE_DELETE(m_pGrass);
	SAFE_DELETE(m_pScreenGrabber);
	SAFE_DELETE(m_ParticleFactory);
	SAFE_DELETE(m_pTimer);
}

