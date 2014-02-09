
#include "GameObject.h"
#include "../Misc/LogManager.h"
#include "../Misc/EngineStats.h"

//----------------------------------------------------------------------------------------
// Purpose  : Constructor
//----------------------------------------------------------------------------------------
GameObject::GameObject()
{
	m_iId		=	0;
	m_vecPos	=	D3DXVECTOR3(0,0,0);
	m_vecRot	=	D3DXVECTOR3(0,0,0);
	m_vecScale	=	D3DXVECTOR3(1,1,1);
	m_bVisible	=	true;

	m_strName.clear();
	m_Type		=	NONE;
}

//----------------------------------------------------------------------------------------
// Purpose  : Destructor
//----------------------------------------------------------------------------------------
GameObject::~GameObject()
{

}

//----------------------------------------------------------------------------------------
// Purpose  : Init
//----------------------------------------------------------------------------------------
void	GameObject::Init()
{

}

//----------------------------------------------------------------------------------------
// Purpose  : Update this GameObject
//----------------------------------------------------------------------------------------
void	GameObject::Update(float dt)
{

}

//----------------------------------------------------------------------------------------
// Purpose  : Render this GameObject
//----------------------------------------------------------------------------------------
void	GameObject::Render()
{

}

//////////////////////////////////////////////////////////////////////////
void	GameObject::SetObjectVisiblilty(bool flag)
{
	m_bVisible = flag;
}

//////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectPos(const D3DXVECTOR3& pos)
{
	//m_vecPos = pos;
	//m_pModel->Translate(&m_vecPos);
}

//////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectRot(const D3DXVECTOR3& rot, float angle)
{
	//m_vecRot = rot;
	//m_pModel->Rotate(&m_vecRot, angle);
}

//////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectScale(const D3DXVECTOR3& sc)
{
	//m_vecScale = sc;
	//m_pModel->Scale(&m_vecScale);
}

//////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectID(int id)
{
	m_iId = id;
}

//////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectName(const std::string& name)
{
	m_strName = name;
}

/////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::setEnvMap(const std::string& envmap)
{
	m_strEnvMap = envmap;
}

/////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::setObjectType(const GameObjectType& type)
{
	m_Type = type;
}

/////////////////////////////////////////////////////////////////////////////////////////
void	GameObject::Kill()
{

}

