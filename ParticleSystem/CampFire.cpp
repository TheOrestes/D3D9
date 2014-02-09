
#include "../Renderables/StaticGeometry.h"
#include "../GeometrySystem/StaticGeometryManager.h"
#include "../Misc/Helper.h"
#include "CampFire.h"

//////////////////////////////////////////////////////////////////////////
CampFire::CampFire(const std::string& fxName, const std::string& techName, 
					const std::string& texName, const D3DXVECTOR3& accel, 
					int maxNumParticles, float timePerParticle) : 
		ParticleSystem(fxName, techName, texName, accel, maxNumParticles, timePerParticle){}

//////////////////////////////////////////////////////////////////////////
void	CampFire::InitParticle(Particle& out)
{
	// initial red
	out.initialColor	=	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// Attach this particle to a particular mesh to get its position
	D3DXVECTOR4 pos = D3DXVECTOR4(0.0f, 100.0f, 0.0f, 1.0f); 
	D3DXVECTOR4* rootPosition = &pos; //D3DXVECTOR4(0.0f, 100.0f, 0.0f, 1.0f); //StaticGeometryManager::GetInstance()->GetGeometryByName("DWARF")->GetWorldPosition();
	out.initialPos		=	D3DXVECTOR3(rootPosition->x, rootPosition->y, rootPosition->z);

	out.initialSize		=	Helper::GetRandomFloat(200.0f, 225.0f);
	out.initialTime		=	m_fTime;
	D3DXVECTOR3 minVel(-1.0f, 5.0f, -1.0f);
	D3DXVECTOR3 maxVel(1.0f, 7.0f, 1.0f);
	Helper::GetRandomVector(out.initialVelocity, minVel, maxVel);
	out.lifeTime		=	Helper::GetRandomFloat(1.0f, 3.0f);
	out.mass			=	0.0f;
}
