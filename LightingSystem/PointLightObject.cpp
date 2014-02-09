
#include "../GeometrySystem/GameObject.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"
#include "PointLightObject.h"

/////////////////////////////////////////////////////////////////////////////////////////
PointLightObject::PointLightObject()
{
	m_vecLightPos	= GameObject::m_vecPos; 
	m_vecLightCol	= D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius		= 50.0f;
	m_fIntensity	= 1.0f;
	m_iAttenuation	= 0;	

	m_pMesh			= NULL;
	m_pEffect		= NULL;	

	m_vecVelocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

/////////////////////////////////////////////////////////////////////////////////////////
PointLightObject::~PointLightObject()
{
	Kill();
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::Init()
{
	HRESULT Hr = 0;

	// Create a sphere representing Point Light
	Hr = D3DXCreateSphere(RenderDevice::GetInstance()->getDevice(),
							2.0f, 32, 32, &m_pMesh,NULL);

	if (S_OK != Hr)
	{
		LogManager::GetInstance()->WriteToFile("PointLightObject Initialization FAILED", "D3DXCreateSphere()");
		return;
	}

	// Create simple ambient shader
	Hr = D3DXCreateEffectFromFile(RenderDevice::GetInstance()->getDevice(),
									"Shaders\\FX\\AmbientLighting.fx", 0, 0, 0, 0,
									&m_pEffect, 0);

	if (S_OK != Hr)
	{
		LogManager::GetInstance()->WriteToFile("PointLightObject Initialization FAILED", "D3DXCreateEffectFromFile()");
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::Render()
{
	UINT totalPasses;
	D3DXHANDLE hTechnique;
	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX projection;
	D3DXMATRIX worldViewProjection;

	hTechnique = m_pEffect->GetTechniqueByName("AmbientLighting");

	if (FAILED(m_pEffect->SetTechnique(hTechnique)))
		return;

	D3DXMatrixTranslation(&world, m_vecLightPos.x, m_vecLightPos.y, m_vecLightPos.z);

	view = *(RenderDevice::GetInstance()->GetGlobalViewMatrix());
	projection = *(RenderDevice::GetInstance()->GetGlobalProjectionMatrix());

	worldViewProjection = world * view * projection;

	m_pEffect->SetMatrix("worldViewProjectionMatrix", &worldViewProjection);
	m_pEffect->SetFloat("ambientIntensity", m_fIntensity);
	m_pEffect->SetVector("ambientColor", &m_vecLightCol);

	// Draw the light object.

	if (SUCCEEDED(m_pEffect->Begin(&totalPasses, 0)))
	{
		for (UINT pass = 0; pass < totalPasses; ++pass)
		{
			if (SUCCEEDED(m_pEffect->BeginPass(pass)))
			{
				m_pMesh->DrawSubset(0);
				m_pEffect->EndPass();
			}
		}

		m_pEffect->End();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::Update(float dt)
{
	m_vecLightPos.x += m_vecVelocity.x;
	m_vecLightPos.y += m_vecVelocity.y;
	m_vecLightPos.z += m_vecVelocity.z;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::Kill()
{
	std::string lightname = getObjectName();
	LogManager::GetInstance()->WriteToFile("Killing ", lightname.c_str());

	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pMesh);
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightColor(const D3DXVECTOR4& col )
{
	m_vecLightCol = col;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightPosition(const D3DXVECTOR3& pos )
{
	m_vecLightPos = pos;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightIntensity( float intensity )
{
	m_fIntensity = intensity;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightRadius( float radius )
{
	m_fRadius = radius;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightAtten( int atten )
{
	m_iAttenuation = atten;
}

/////////////////////////////////////////////////////////////////////////////////////////
void PointLightObject::setLightVelocity(const D3DXVECTOR3& vel )
{
	m_vecVelocity = vel;
}



