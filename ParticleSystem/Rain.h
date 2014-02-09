// ***************************************************************
//  Rain			
//  -------------------------------------------------------------
//  Purpose :	Rain Particle System
// ***************************************************************

#ifndef _RAIN_H_
#define _RAIN_H_

#include "ParticleSystem.h"

class Rain : public ParticleSystem
{
public:
	Rain(const std::string& fxName, const std::string& techName, 
			const std::string& texName, const D3DXVECTOR3& accel, 
			int maxNumParticles, float timePerParticle);

	void	InitParticle(Particle& out);
};

#endif