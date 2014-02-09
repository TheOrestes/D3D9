// ***************************************************************
//  DiwaliPaus		
//  -------------------------------------------------------------
//  Purpose :	Effect class for Diwali Paus ( firecracker )
// ***************************************************************


#ifndef DIWALI_PAUS_H
#define DIWALI_PAUS_H

#include "ParticleSystem.h"

class DiwaliPaus	:	public ParticleSystem
{
public:
	DiwaliPaus(const std::string& fxName, const std::string& techName, 
				const std::string& texName, const D3DXVECTOR3& accel, 
				int numParticles, float timePerParticle);

	void	InitParticle(Particle& out);
};

#endif