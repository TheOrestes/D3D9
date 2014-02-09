// ***************************************************************
//  ParticleSystem		
//  -------------------------------------------------------------
//  Purpose :	
// ***************************************************************

#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include <string>
#include <vector>
#include "../GeometrySystem/Vertex.h"

class ParticleSystem
{
public:
	ParticleSystem( const std::string& fxName, const std::string& techName, 
					const std::string& texName, const D3DXVECTOR3& accel, 
					int maxNumParticles, float timePerParticle);

	~ParticleSystem();

	void					SetWorldMatrix(const D3DXMATRIX& world);
	void					AddParticle();

	virtual void			InitParticle(Particle& out) = 0;
	virtual void			Update(float dt);
	virtual void			Render();

	inline float			GetTime(){ return m_fTime; }
	float					SetTime(float t){ m_fTime = t; }	


protected:
	LPD3DXEFFECT			m_pFX;
	LPDIRECT3DTEXTURE9		m_pTex;						// Particle texture
	LPDIRECT3DVERTEXBUFFER9	m_pVB;						// Vertex buffer for Particle

	D3DXHANDLE				m_hTech;
	D3DXHANDLE				m_hMatWVP;
	D3DXHANDLE				m_hMatWorld;
	D3DXHANDLE				m_hMatView;
	D3DXHANDLE				m_hMatProj;
	D3DXHANDLE				m_hVecEye;
	D3DXHANDLE				m_hTex;
	D3DXHANDLE				m_hTime;
	D3DXHANDLE				m_hAccel;
	D3DXHANDLE				m_hViewportHeight;

	D3DXMATRIX				m_MatWorld;					
	D3DXMATRIX				m_MatInvWorld;
	float					m_fTime;					// how long particle system has been running?
	D3DXVECTOR3				m_VecAccel;					// constant acceleration vector
	int						m_iMaxParticles;			
	float					m_fTimePerParticle;			// time that should pass before system emits another particle, -ve value for event triggered particles ( like gunfire or car collision ) 

	std::vector<Particle>	m_Particles;				// list of every particle in the system
	std::vector<Particle*>	m_AliveParticles;			// particles to render this frame
	std::vector<Particle*>	m_DeadParticles;	
};


#endif


