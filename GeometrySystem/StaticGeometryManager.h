// ***************************************************************
//  StaticGeometryManager			
//  -------------------------------------------------------------
//  Purpose :	Manages all Static Geometry Objects in the scene
//	manages creation & deletion at runtime through STL usage.
//  staticGeomMgr will call render on all the object only once & 
//  based on individual object's rendering states, object will be 
//  rendered.
// ***************************************************************

#ifndef _STATIC_GEOM_MGR_H_
#define _STATIC_GEOM_MGR_H_

#include <Windows.h>
#include <map>
#include <list>
#include "../Misc/Helper.h"

class StaticGeometry;
class TextParser;

class StaticGeometryManager
{
public:
	// Singleton Instance
	static StaticGeometryManager* GetInstance()
	{
		static StaticGeometryManager inst;
		return &inst;
	}
	
	~StaticGeometryManager();

	void			Initialize();
	void			AddToStaticGeometryList(const std::string& fileName, const std::string& meshName);
	void			AddToStaticGeometryList(const std::string& fileName, const Helper::StaticGeomData& data);
	void			RemoveFromStaticGeometryList(StaticGeometry* geom);
	void			RemoveFromStaticGeometryList(const std::string& meshName);
	StaticGeometry*	GetGeometryByName(const std::string& meshName);
	void			preRender();
	void			Render();
	void			postRender();
	void			Update(float dt);
	void			Kill();

private:
	StaticGeometryManager();

	StaticGeometryManager(const StaticGeometryManager&);				// Explicitly disallow the use of 
	StaticGeometryManager operator=(const StaticGeometryManager&);		// compiler-generated functions  

	std::map<StaticGeometry*, std::string> m_GeomList;
};

#endif

