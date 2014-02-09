#ifndef SPRINKLER_H
#define SPRINKLER_H

#include "ParticleSystem.h"

class Sprinkler : public ParticleSystem
{
public:
	Sprinkler(const std::string& fxName, const std::string& techName, 
				const std::string& texName, const D3DXVECTOR3& accel, 
				int numParticles, float timePerParticle);
	
	void	InitParticle(Particle& out);
};

#endif

