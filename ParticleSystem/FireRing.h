// ***************************************************************
//  FireRing			
//  -------------------------------------------------------------
//  Purpose :	Ring of Fire
// ***************************************************************

#ifndef _FIRE_RING_H_
#define _FIRE_RING_H_

#include "ParticleSystem.h"

class FireRing : public ParticleSystem
{
public:
	FireRing(const std::string& fxName, const std::string& techName, 
				const std::string& texName, const D3DXVECTOR3& accel, 
				int maxNumParticles, float timePerParticle);

	void	InitParticle(Particle& out);
};

#endif