
#include "../Misc/Helper.h"
#include "FireRing.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
FireRing::FireRing(const std::string& fxName, const std::string& techName, 
					const std::string& texName, const D3DXVECTOR3& accel, 
					int maxNumParticles, float timePerParticle) : 
	ParticleSystem(fxName, techName, texName, accel, maxNumParticles, timePerParticle)
	{}

//------------------------------------------------------------------------
// Purpose  : Initialization
//------------------------------------------------------------------------
void	FireRing::InitParticle(Particle &out)
{
	// time particle is created wrt global system
	out.initialTime = m_fTime;

	out.lifeTime	= Helper::GetRandomFloat(2.0f, 4.0f);
	out.initialSize = Helper::GetRandomFloat(10.0f, 15.0f);

	// random velocity
	Helper::GetRandomVec(out.initialVelocity);

	// value used by shaderas an amplitude factor
	out.mass = Helper::GetRandomFloat(1.0f, 2.0f);

	out.initialColor = Helper::SNOW_WHITE_COLOR() * Helper::GetRandomFloat(0.5f, 1.0f);

	// Generate random particle on the ring in polar coordinates:
	// random radius and random angle.
	float r = Helper::GetRandomFloat(10.0f, 14.0f);
	float t = Helper::GetRandomFloat(0, 2.0f*D3DX_PI);

	// Convert to Cartesian coordinates.
	out.initialPos.x = r*cosf(t);
	out.initialPos.y = r*sinf(t);

	// Random depth value in [-1, 1] (depth of the ring)
	out.initialPos.z = Helper::GetRandomFloat(-1.0f, 1.0f);
}