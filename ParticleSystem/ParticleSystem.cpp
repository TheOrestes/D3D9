
#include "ParticleSystem.h"
#include "../Game/Camera.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : Globals
//-----------------------------------------------------------------------

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
ParticleSystem::ParticleSystem(const std::string& fxName, const std::string& techName, 
								const std::string& texName, const D3DXVECTOR3 &accel, 
								int maxNumParticles, float timePerParticle)
{
	m_VecAccel = accel;
	m_fTime = 0.0f;
	m_iMaxParticles = maxNumParticles;
	m_fTimePerParticle = timePerParticle;

	// Allocate memory for maximum number of particles
	m_Particles.resize(m_iMaxParticles);
	m_AliveParticles.reserve(m_iMaxParticles);
	m_DeadParticles.reserve(m_iMaxParticles);

	// Make all particles DEAD at start!
	for (int i = 0 ; i<m_iMaxParticles ; i++)
	{
		m_Particles[i].lifeTime = 0.0f;
		m_Particles[i].initialTime = 0.0f;
	}

	D3DXMatrixIdentity(&m_MatWorld);
	D3DXMatrixIdentity(&m_MatInvWorld);

	// Create the FX
	LPDIRECT3DDEVICE9	device = RenderDevice::GetInstance()->getDevice();
	LPD3DXBUFFER	err = NULL;
	HRESULT Hr = D3DXCreateEffectFromFile(device, fxName.c_str(), 0, 0, 0, 0, &m_pFX, &err );
	
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("Particle Effect creation FAILED!", (char*)err->GetBufferPointer());
	}
		
	// Create the texture
	Hr = D3DXCreateTextureFromFile(device, texName.c_str(), &m_pTex);
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("Particle Texture creation FAILED!");
	}

	// Create Vertex Buffer
	Hr = device->CreateVertexBuffer(maxNumParticles*sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 0, D3DPOOL_DEFAULT, &m_pVB, 0);
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("Particle Vertex BUffer creation FAILED!");
	}

	// Assign all the Shader Handles
	m_hAccel			=	m_pFX->GetParameterByName(0, "gAccel");
	m_hMatWVP			=	m_pFX->GetParameterByName(0, "matWVP");
	m_hMatWorld			=	m_pFX->GetParameterByName(0, "matWorld");
	m_hMatView			=	m_pFX->GetParameterByName(0, "matView");
	m_hMatProj			=	m_pFX->GetParameterByName(0, "matProj");
	m_hTech				=	m_pFX->GetTechniqueByName(techName.c_str());
	m_hTex				=	m_pFX->GetParameterByName(0, "gTex");
	m_hTime				=	m_pFX->GetParameterByName(0, "gTime");
	m_hVecEye			=	m_pFX->GetParameterByName(0, "gCamPos");
	m_hViewportHeight	=	m_pFX->GetParameterByName(0, "gViewportHeight");

	// WE don't need to set these every frame since they dont change!
	m_pFX->SetTechnique(m_hTech);
	m_pFX->SetValue(m_hAccel, m_VecAccel, sizeof(D3DXVECTOR3));
	m_pFX->SetTexture(m_hTex, m_pTex);
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
ParticleSystem::~ParticleSystem()
{
	SAFE_RELEASE(m_pFX);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pVB);
}

//------------------------------------------------------------------------
// Purpose  : Set World & WorldInverse Matrix
//------------------------------------------------------------------------
void	ParticleSystem::SetWorldMatrix(const D3DXMATRIX& world)
{
	m_MatWorld = world;
	D3DXMatrixInverse(&m_MatInvWorld, 0, &m_MatWorld);
}

//------------------------------------------------------------------------
// Purpose  : Add particle
//------------------------------------------------------------------------
void	ParticleSystem::AddParticle()
{
	// check if we have any dead particles
	if (m_DeadParticles.size() > 0)
	{
		// Reinitialize a particle
		Particle* p = m_DeadParticles.back();
		InitParticle(*p);

		// Now its not dead
		m_DeadParticles.pop_back();
		m_AliveParticles.push_back(p);
	}
}

//------------------------------------------------------------------------
// Purpose  : Update Particle System
//------------------------------------------------------------------------
void	ParticleSystem::Update(float dt)
{
	m_fTime += dt;

	// Rebuild the dead & alive list
	m_DeadParticles.resize(0);
	m_AliveParticles.resize(0);

	// for each particle
	for (int i = 0 ; i <m_iMaxParticles ; i++)
	{
		// Check if particle is dead/alive?
		if ((m_fTime - m_Particles[i].initialTime) > m_Particles[i].lifeTime)
		{
			m_DeadParticles.push_back(&m_Particles[i]);
		}
		else
		{
			m_AliveParticles.push_back(&m_Particles[i]);
		}
	}

	// negative or zero value for m_fTimePerParticle indicates not to emit any particle
	if(m_fTimePerParticle > 0.0f)
	{
		// Emit particle
		static float accumTime = 0.0f;
		accumTime += dt;
		while (accumTime >= m_fTimePerParticle)
		{
			AddParticle();
			accumTime -= m_fTimePerParticle;
		}
	}
}

//------------------------------------------------------------------------
// Purpose  : Render the particle System
//------------------------------------------------------------------------
void	ParticleSystem::Render()
{
	LPDIRECT3DDEVICE9	device = RenderDevice::GetInstance()->getDevice();

	// Grab camera position
	D3DXVECTOR3 eyePosW = Camera::GetInstance()->m_Position;

	// Now make it relative to Particle System's local space
	D3DXVECTOR3 eyePosL;
	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &m_MatInvWorld);

	D3DXMATRIX* matView = RenderDevice::GetInstance()->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = RenderDevice::GetInstance()->GetGlobalProjectionMatrix();

	// Create ViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;
	D3DXMATRIX matViewProj = matViewVal * matProjVal;

	// Set FX params
	m_pFX->SetValue(m_hVecEye, &eyePosW, sizeof(D3DXVECTOR3));
	m_pFX->SetFloat(m_hTime, m_fTime);
	m_pFX->SetMatrix(m_hMatWVP, &matViewProj);
	m_pFX->SetMatrix(m_hMatWorld, &m_MatWorld);
	m_pFX->SetMatrix(m_hMatView, &matViewVal);
	m_pFX->SetMatrix(m_hMatProj, &matProjVal);

	// Draw
	UINT nPasses = 0;
	m_pFX->Begin(&nPasses, 0);
	m_pFX->BeginPass(0);
	
	device->SetStreamSource(0, m_pVB, 0, sizeof(Particle));
	device->SetVertexDeclaration(Particle::pVertDecl);

	Particle* p = NULL;

	m_pVB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD);
	int vbIndex = 0;

	// for each alive particle
	for (UINT i = 0 ; i< m_AliveParticles.size() ; i++)
	{
		// copy particle to Vertex Buffer
		p[vbIndex] = *m_AliveParticles[i];
		vbIndex++;
	}
	m_pVB->Unlock();

	// Render as many particles we copied
	if (vbIndex > 0)
	{
		device->DrawPrimitive(D3DPT_POINTLIST, 0, vbIndex);
	}

	m_pFX->EndPass();
	m_pFX->End();
}










































