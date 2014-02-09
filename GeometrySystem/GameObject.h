
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "IObject.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

//struct Model;

enum GameObjectType
{
	NONE,
	STATIC_MESH,
	SKYBOX,
	TERRAIN,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	PARTICLE_SYSTEM
};

class GameObject : public IObject
{
public:
	GameObject();
	virtual ~GameObject();

	// Override Interface Functions!
	virtual	void			Update(float dt);
	virtual void			Render();

	virtual void			Init();
	virtual void			Kill();

	inline bool				IsModelVisible()	{ return m_bVisible; }
	inline int				getObjectID()		{ return m_iId; }
	inline std::string		getObjectName()		{ return m_strName; }
	inline GameObjectType	getObjectType()		{ return m_Type; }

	inline D3DXVECTOR3&		getObjectPos()		{ return m_vecPos; }
	inline D3DXVECTOR3&		getObjectRot()		{ return m_vecRot; }
	inline D3DXVECTOR3&		getObjectScale()	{ return m_vecScale; }
	unsigned int			getObjectStats()	{ return m_bTriangleCount; }
	inline std::string		getEnvMap()			{ return m_strEnvMap; }

	// SETTER
	void					setObjectID(int id);
	void					setObjectName(const std::string& name);
	void					setObjectType(const GameObjectType& type);
	void					setEnvMap(const std::string& envmap);

	virtual void			setObjectPos(const D3DXVECTOR3& pos);
	virtual void			setObjectRot(const D3DXVECTOR3& rot, float angle);
	virtual void			setObjectScale(const D3DXVECTOR3& sc);
	virtual void			SetObjectVisiblilty(bool flag);




private:
	int						m_iId;
	std::string				m_strName;
	GameObjectType			m_Type;
	unsigned int			m_bTriangleCount;

protected:
	D3DXVECTOR3				m_vecPos;
	D3DXVECTOR3				m_vecRot;
	D3DXVECTOR3				m_vecScale;
	bool					m_bVisible;
	std::string				m_strEnvMap;
};

#endif