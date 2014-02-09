#include "Skybox.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Game/Camera.h"
#include "../Misc/Helper.h"
#include "../Misc/EngineStats.h"
#include "../Misc/LogManager.h"

//------------------------------------------------------------------------
// Date     : 16/2/2010
// Purpose  : Constructor
//-----------------------------------------------------------------------
Skybox::Skybox()
{
	m_pCubeTex		=	NULL;
	m_pFX			=	NULL;
	m_pSkySPhere	=	NULL;

	m_fRadius		=	0.0f;
}

//------------------------------------------------------------------------
// Date     : 30/6/2012
// Purpose  : Initialize Skybox here
//------------------------------------------------------------------------
void	Skybox::InitializeSky(const std::string& fileName, float radius)
{
	LPDIRECT3DDEVICE9 device = RenderDevice::GetInstance()->getDevice();

	m_fRadius = radius;

	D3DXCreateSphere(device, m_fRadius, 30, 30, &m_pSkySPhere, 0);
	D3DXCreateCubeTextureFromFile(device, fileName.c_str(), &m_pCubeTex);

	LPD3DXBUFFER	errors = NULL;
	HRESULT Hr = D3DXCreateEffectFromFile(device, "Shaders\\FX\\sky.fx", 0,0,0,0, &m_pFX, &errors);
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("ERROR -----> sky.fx() FAILED", (char*)errors->GetBufferPointer());
		return;
	}

	m_hTech   = m_pFX->GetTechniqueByName("SkyTech");
	m_hWVP    = m_pFX->GetParameterByName(0, "matWVP");
	m_hEnvMap = m_pFX->GetParameterByName(0, "gEnvMap");
	m_hDelta  = m_pFX->GetParameterByName(0, "delta");
	m_hTime   = m_pFX->GetParameterByName(0, "fTime");

	m_pFX->SetTechnique(m_hTech);
	m_pFX->SetTexture(m_hEnvMap, m_pCubeTex);

	LogManager::GetInstance()->WriteToFile("Sky Initialized Successfully...");

	UpdateEngineStats();
}

//------------------------------------------------------------------------
// Date     : 30/6/2012
// Purpose  : Update Engine Stats
//------------------------------------------------------------------------
void Skybox::UpdateEngineStats()
{
	DWORD _tris = m_pSkySPhere->GetNumFaces();
	EngineStats::GetInstance()->addTriangles(_tris);

	DWORD _verts = m_pSkySPhere->GetNumVertices();
	EngineStats::GetInstance()->addVertices(_verts);
}

//------------------------------------------------------------------------
// Date     : 16/2/2010
// Purpose  : Destructor
//------------------------------------------------------------------------
Skybox::~Skybox()
{
	ShutDown();
}

//------------------------------------------------------------------------
// Date     : 30/6/2012
// Purpose  : Update the SKY!
//------------------------------------------------------------------------
void Skybox::Update(float dt)
{
	delta = dt;
	fTime += delta;
}

//------------------------------------------------------------------------
// Date     : 16/2/2010
// Purpose  : Render Skybox
//------------------------------------------------------------------------
void	Skybox::RenderSkybox()
{
	LPDIRECT3DDEVICE9 device = RenderDevice::GetInstance()->getDevice();
	
	// Sky is always centered around Camera's position
	D3DXMATRIX	W;
	D3DXVECTOR3 P = Camera::GetInstance()->m_Position;
	D3DXMatrixTranslation(&W, P.x, P.y, P.z);

	D3DXMATRIX* matView = RenderDevice::GetInstance()->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = RenderDevice::GetInstance()->GetGlobalProjectionMatrix();

	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	D3DXMATRIX mViewProjection = matViewVal * matProjVal;
	m_pFX->SetMatrix(m_hWVP, &(W * mViewProjection));

	m_pFX->SetFloat(m_hDelta, delta);
	m_pFX->SetFloat(m_hTime, fTime);
	m_pFX->CommitChanges();

	UINT nPasses = 0;
	m_pFX->Begin(&nPasses, 0);
	m_pFX->BeginPass(0);
	m_pSkySPhere->DrawSubset(0);
	m_pFX->EndPass();
	m_pFX->End();
}

//------------------------------------------------------------------------
// Date     : 16/2/2010
// Purpose  : Destroy!
//------------------------------------------------------------------------
void	Skybox::ShutDown()
{
	SAFE_RELEASE(m_pSkySPhere);
	SAFE_RELEASE(m_pCubeTex);
	SAFE_RELEASE(m_pFX);
	LogManager::GetInstance()->WriteToFile("Skybox Shutdown...");
}


