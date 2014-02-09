// ***************************************************************
//  ParticleFactory			
//  -------------------------------------------------------------
//  Purpose :	Manage overall particle systems in a world, uses
//				factory design pattern!!!
// ***************************************************************


#ifndef PARTICLE_FACTORY_H
#define PARTICLE_FACTORY_H

#include "ParticleType.h"

class ParticleFactory
{
public:
	// Factory method
	static	ParticleSystem*	MakeParticleSystem(int choice);
};

#endif