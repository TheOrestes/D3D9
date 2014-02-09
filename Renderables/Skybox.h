// ***************************************************************
//  Skybox			
//  -------------------------------------------------------------
//  Purpose :	Handle rendering for Skybox
// ***************************************************************

#ifndef SKYBOX_H
#define SKYBOX_H

#include <d3dx9.h>
#include <string>

class Skybox 
{
public:
	Skybox();
	~Skybox();

	void		InitializeSky(const std::string& fileName, float radius);
	void		RenderSkybox();
	void		ShutDown();
	void		Update(float dt);
	void		UpdateEngineStats();

	inline LPDIRECT3DCUBETEXTURE9	getEnvMap(){ return m_pCubeTex; }
	inline float					getRadius(){ return m_fRadius; }
	
private:
	LPD3DXMESH				m_pSkySPhere;
	float					m_fRadius;

	float					fTime;
	float					delta;

	LPDIRECT3DCUBETEXTURE9	m_pCubeTex;
	LPD3DXEFFECT			m_pFX;
	D3DXHANDLE				m_hTech;
	D3DXHANDLE				m_hEnvMap;
	D3DXHANDLE				m_hWVP;
	D3DXHANDLE				m_hDelta;
	D3DXHANDLE				m_hTime;
};


#endif