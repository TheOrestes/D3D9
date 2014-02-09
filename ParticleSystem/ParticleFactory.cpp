#include "ParticleSystem.h"
#include "CampFire.h"
#include "Rain.h"
#include "Sprinkler.h"
#include "FireRing.h"
#include "DiwaliPaus.h"
#include "ParticleFactory.h"

//------------------------------------------------------------------------
// Purpose  : Factory Method
//------------------------------------------------------------------------
ParticleSystem* ParticleFactory::MakeParticleSystem(int choice)
{
	switch (choice)
	{
	case PTYPE_FIRERING:
		return static_cast<ParticleSystem*>(new FireRing("Shaders\\FX\\firering.fx", "FireRingTech", "Data\\ParticleData\\torch.dds", D3DXVECTOR3(0.0f, 0.9f, 0.0f), 1500, 0.0025f));
		break;

	case PTYPE_CAMPFIRE:
		return static_cast<ParticleSystem*>(new CampFire("Shaders\\FX\\CampFire.fx", "CampFireTech", "Data\\ParticleData\\star.jpg", D3DXVECTOR3(0.0f, 1.2f, 0.0f), 2000, 0.075f));
		break;

	case PTYPE_SPRINKLER:
		return static_cast<ParticleSystem*>(new Sprinkler("Shaders\\FX\\sprinkler.fx", "SprinklerTech", "Data\\ParticleData\\star.jpg", D3DXVECTOR3(0.f, 0.f, 0.f), 2000, 0.0002f));
		break;

	case PTYPE_RAIN:
		return static_cast<ParticleSystem*>(new Rain("Shaders\\FX\\rain.fx", "RainTech", "Data\\ParticleData\\raindrop.dds", D3DXVECTOR3(-1.0f, -9.8f, 0.0f), 4000, 0.001f));
		break;

	case PTYPE_DIWALIPAUS:
		return static_cast<ParticleSystem*>(new DiwaliPaus("Shaders\\FX\\DiwaliPaus.fx", "DiwaliTech", "Data\\ParticleData\\star.jpg", D3DXVECTOR3(0.0f, -9.8f, 0.0f), 4000, 0.001f));

	default:
		return static_cast<ParticleSystem*>(new Rain("Shaders\\FX\\rain.fx", "RainTech", "Data\\ParticleData\\raindrop.dds", D3DXVECTOR3(-1.0f, -9.8f, 0.0f), 4000, 0.001f));
		break;
	}
}