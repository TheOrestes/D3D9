
#include "../Misc/Helper.h"
#include "Sprinkler.h"

//////////////////////////////////////////////////////////////////////////
Sprinkler::Sprinkler(const std::string& fxName, const std::string& techName, 
						const std::string& texName, const D3DXVECTOR3& accel, 
						int numParticles, float timePerParticle):	  
	ParticleSystem(fxName, techName, texName, accel, numParticles, timePerParticle){}

//////////////////////////////////////////////////////////////////////////
void	Sprinkler::InitParticle(Particle& out)
{
	out.initialColor		=	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	out.initialPos			=	D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	out.initialSize			=	Helper::GetRandomFloat(10.0f, 20.0f);
	out.initialTime			=	m_fTime;
	D3DXVECTOR3 minVec(-1.0f, -1.0f, -1.0f);
	D3DXVECTOR3 maxVec(1.0f, 1.0f, 1.0f);
	Helper::GetRandomVector(out.initialVelocity, minVec, maxVec);
	out.lifeTime			=	Helper::GetRandomFloat(10.5f, 10.8f);
	out.mass				=	Helper::GetRandomFloat(0.8f, 1.2f);
};