#include "../RenderDevice/RenderDevice.h"
#include "../GeometrySystem/StaticGeometryManager.h"
#include "../Renderables/StaticGeometry.h"
#include "../Misc/LogManager.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
StaticGeometryManager::StaticGeometryManager()
{
	// Make sure everything is clean n clear!
	m_GeomList.clear();
}

//------------------------------------------------------------------------ 
// Purpose  : Destructor
//------------------------------------------------------------------------
StaticGeometryManager::~StaticGeometryManager()
{
	// Manager dying, need to cleanup all Static Geometry before!
	Kill();
	m_GeomList.clear();
}

//------------------------------------------------------------------------
// Purpose  : Initialize Static Geometry Manager
//------------------------------------------------------------------------
void	StaticGeometryManager::Initialize()
{
	// get a list of string containing file names with path,
	// pass it on for per geom per file basis in below function
	// LATER : add support for Position!
	// m_GeomList[i].Initialize();

	// Read the file here & extract the filename one by one
	// store it in the list
	


	//std::list<StaticGeometry*>::iterator iter = m_GeomList.begin();

	//for (;iter != m_GeomList.end() ; iter++)
	//{
		
	//}

}

//------------------------------------------------------------------------
// Purpose  : preRender all static Geometry
//------------------------------------------------------------------------
void	StaticGeometryManager::preRender()
{	
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	for (;iter != m_GeomList.end() ; iter++)
	{
		iter->first->preRender();
	}
}

//------------------------------------------------------------------------
// Purpose  : Render all Static Geometry
//------------------------------------------------------------------------
void	StaticGeometryManager::Render()
{
	LPDIRECT3DDEVICE9 device = RenderDevice::GetInstance()->getDevice();

	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	for (; iter != m_GeomList.end() ; iter++)
	{
		iter->first->SetShaderVariables();
		iter->first->Render();
	}
}

//------------------------------------------------------------------------
// Purpose  : postRender all static Geometry
//------------------------------------------------------------------------
void	StaticGeometryManager::postRender()
{
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	for (; iter != m_GeomList.end() ; iter++)
	{
		iter->first->PostRender();
	}
}

//------------------------------------------------------------------------
// Purpose  : Update all Static Geometries in the scene
//------------------------------------------------------------------------
void	StaticGeometryManager::Update(float dt)
{
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	for (; iter != m_GeomList.end() ; iter++)
	{
		iter->first->Update(dt);
	}
}

//------------------------------------------------------------------------
// Purpose  : Kill all Static Geometry
//------------------------------------------------------------------------
void	StaticGeometryManager::Kill()
{
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	for (; iter != m_GeomList.end() ; iter++)
	{
		iter->first->Kill();
		iter->second.clear();
	}
	
	LogManager::GetInstance()->WriteToFile("StaticGeometryManager Shutdown...");
}

//------------------------------------------------------------------------ 
// Purpose  : Push the Static Geometry into Manager
//------------------------------------------------------------------------
void	StaticGeometryManager::AddToStaticGeometryList(const std::string& fileName, 
														const std::string& meshName)
{
	StaticGeometry*	tempGeom = new StaticGeometry();

	if (tempGeom != NULL)
	{
		tempGeom->Initialize(fileName);
		m_GeomList.insert(std::make_pair(tempGeom, meshName));
	}
}

//------------------------------------------------------------------------
// Purpose  : Push the Static Geometry into Manager from Text File
//------------------------------------------------------------------------
void	StaticGeometryManager::AddToStaticGeometryList(const std::string& fileName, 
														const Helper::StaticGeomData& data)
{
	StaticGeometry*	tempGeom = new StaticGeometry();

	if (tempGeom != NULL)
	{
		if(tempGeom->Initialize(data))
			m_GeomList.insert(std::make_pair(tempGeom, fileName));
	}
}

//------------------------------------------------------------------------
// Purpose  : Get the StaticGeometry pointer by Name in the list
//------------------------------------------------------------------------
StaticGeometry*	StaticGeometryManager::GetGeometryByName(const std::string& meshName)
{
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();

	while (iter->second.compare(meshName))
	{
		iter++;
	}

	return iter->first;
}

//------------------------------------------------------------------------ 
// Purpose  : Remove Static Geometry from the Manager
//------------------------------------------------------------------------
void	StaticGeometryManager::RemoveFromStaticGeometryList(StaticGeometry*	geom)
{
	if (geom != NULL)
	{
		std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();
		while (iter->first != geom)
		{
			iter++;
		}

		m_GeomList.erase(iter);
	}
}

//------------------------------------------------------------------------
// Purpose  : Remove static geometry from the Manager by Name
//------------------------------------------------------------------------
void	StaticGeometryManager::RemoveFromStaticGeometryList(const std::string& meshName)
{
	std::map<StaticGeometry*, std::string>::iterator iter = m_GeomList.begin();
	while(iter->second.compare(meshName))
	{
		iter++;
	}

	m_GeomList.erase(iter);
}

