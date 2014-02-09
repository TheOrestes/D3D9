
#include "DiwaliPaus.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : constructor
//------------------------------------------------------------------------
DiwaliPaus::DiwaliPaus(const std::string& fxName, const std::string& techName, 
						const std::string& texName, const D3DXVECTOR3& accel, 
						int numParticles, float timePerParticle):	  
			ParticleSystem(fxName, techName, texName, accel, numParticles, timePerParticle){}

//------------------------------------------------------------------------
// Purpose  : Initialize particle
//------------------------------------------------------------------------
void	DiwaliPaus::InitParticle(Particle& out)
{
	// Randomize Color
	D3DXVECTOR3	minCol(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 maxCol(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 outCol;
	Helper::GetRandomVector(outCol, minCol, maxCol);
	out.initialColor		=	D3DXCOLOR(outCol.x, outCol.y, outCol.z, 1.0f);
	
	out.initialPos			=	D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	out.initialSize			=	Helper::GetRandomFloat(10.0f, 20.0f);
	out.initialTime			=	m_fTime;

	// Randomize Velocity
	D3DXVECTOR3 minVec(-1.5f, 10.0f, -1.5f);
	D3DXVECTOR3 maxVec(1.5f, 20.0f, 1.5f);
	Helper::GetRandomVector(out.initialVelocity, minVec, maxVec);

	out.lifeTime			=	Helper::GetRandomFloat(5.0f, 7.0f);
	out.mass				=	Helper::GetRandomFloat(0.8f, 1.2f);
};