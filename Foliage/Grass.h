
// ***************************************************************
//  Grass			
//  -------------------------------------------------------------
//  Purpose :	Build Grass!
// ***************************************************************

#ifndef GRASS_H
#define GRASS_H

#include "../GeometrySystem/Vertex.h"
#include <string>

class BruteForceTerrain;

class Grass
{
public:
	Grass();
	virtual ~Grass();

	bool	InitializeGrass(const std::string& textureName, BruteForceTerrain* ptrTerrain);
	void	BuildGrassFin(GrassVertex* v, WORD* k, int& indexOffset, D3DXVECTOR3& worldPos, D3DXVECTOR3& scale);
	bool	BuildGrass();
	void	RenderGrass();
	void	UpdateGrass(float dt);
	void	UpdateEngineStats();
	void	Kill();

private:
	LPDIRECT3DVERTEXBUFFER9	pGrassVB;
	LPDIRECT3DINDEXBUFFER9	pGrassIB;
	LPDIRECT3DTEXTURE9		pGrassTex;
	LPD3DXEFFECT			pEffect;
	LPD3DXMESH				pGrassMesh;

	D3DXMATRIX				mWorld;
	D3DXMATRIX				mProjection;
	D3DXMATRIX				mView;
	D3DXMATRIX				mWorldViewProjection;
	D3DXMATRIX				mViewProjection;
	D3DXVECTOR4				mVecEye;

	D3DXHANDLE				hMatWVP;
	D3DXHANDLE				hTechnique;
	D3DXHANDLE				hDelta;
	D3DXHANDLE				hTime;
	D3DXHANDLE				hGrassTex0;
	D3DXHANDLE				hMatViewProj;
	D3DXHANDLE				hEyePos;

	float					fTime;
	float					delta;

	BruteForceTerrain*		pTerrain;			// Grass is specific to a terrain
};

#endif