
#include "../Misc/Helper.h"
#include "../Game/Camera.h"
#include "Rain.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Rain::Rain(const std::string& fxName, const std::string& techName, 
			const std::string& texName, const D3DXVECTOR3& accel, 
			int maxNumParticles, float timePerParticle) 
	:ParticleSystem(fxName, techName, texName, accel, maxNumParticles, timePerParticle){}

//------------------------------------------------------------------------
// Purpose  : Initialization
//------------------------------------------------------------------------
void Rain::InitParticle(Particle &out)
{
	// Generate particles around camera
	out.initialPos = Camera::GetInstance()->m_Position;

	// speed particles around XZ plane
	out.initialPos.x += Helper::GetRandomFloat(-100.0f, 100.0f);
	out.initialPos.z += Helper::GetRandomFloat(-100.0f, 100.0f);

	// Generate above the camera
	out.initialPos.y += Helper::GetRandomFloat(50.0f, 55.0f);

	out.initialSize = Helper::GetRandomFloat(6.0f, 7.0f);
	out.initialColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	out.initialTime = m_fTime;
	out.lifeTime = Helper::GetRandomFloat(5.5f, 7.5f);

	// Initial falling down velocity
	out.initialVelocity.x = Helper::GetRandomFloat(-1.5f, 0.0f);
	out.initialVelocity.y = Helper::GetRandomFloat(-50.0f, -45.0f);
	out.initialVelocity.z = Helper::GetRandomFloat(-0.5f, 0.5f);
}