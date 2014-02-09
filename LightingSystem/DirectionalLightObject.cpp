
#include "../GeometrySystem/GameObject.h"
#include "DirectionalLightObject.h"

//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::DirectionalLightObject()
{
	m_vecLightDir	= D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_vecChangeDir  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fIntensity	= 1.0f;
	m_vecLightCol	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////
DirectionalLightObject::~DirectionalLightObject()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
void	DirectionalLightObject::Update(float dt)
{
	m_vecLightDir.x += m_vecChangeDir.x;
	m_vecLightDir.y += m_vecChangeDir.y;
	m_vecLightDir.z += m_vecChangeDir.z;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	DirectionalLightObject::setLightColor(const D3DXVECTOR4& changeCol)
{
	m_vecLightCol = changeCol;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	DirectionalLightObject::setLightDirection(const D3DXVECTOR3& dir)
{
	m_vecLightDir = dir;
}

//////////////////////////////////////////////////////////////////////////////////////////
void	DirectionalLightObject::setLightIntensity(float intensity)
{
	m_fIntensity = intensity;
}

//////////////////////////////////////////////////////////////////////////////////////////
void DirectionalLightObject::setLightChangeDir(const D3DXVECTOR3& changeDir )
{
	m_vecChangeDir = changeDir;
}
