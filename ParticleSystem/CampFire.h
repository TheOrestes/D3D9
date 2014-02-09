// ***************************************************************
//  ParticleSystem			
//  -------------------------------------------------------------
//  Purpose :	Create a campfire effect
// ***************************************************************

#ifndef CAMPFIRE_H
#define CAMPFIRE_H

#include "ParticleSystem.h"

class CampFire : public ParticleSystem
{
public:
	CampFire(const std::string& fxName, const std::string& techName, 
			const std::string& texName, const D3DXVECTOR3& accel, int maxNumParticles, float timePerParticle);

	void	InitParticle(Particle& out);
};

#endif