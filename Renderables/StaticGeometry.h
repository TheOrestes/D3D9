// ***************************************************************
//  StaticGeometry			  
//  -------------------------------------------------------------
//  Purpose :	This class handles Static Geometry rendering.
//	Static Geometry represents, walls, crates which wont move or 
//	update with time, hence Update function is not required. 
// ***************************************************************

#ifndef _STATIC_GEOMETRY_H_
#define _STATIC_GEOMETRY_H_

#include <d3dx9.h>
#include <vector>
#include <string>

#include "../Misc/D3DHelper.h"
#include "../Misc/Helper.h"

class FxShader;

class StaticGeometry
{
public:
	StaticGeometry();
	~StaticGeometry();

	bool			Initialize(const std::string& filename);
	bool			Initialize(const Helper::StaticGeomData& data);
	void			preRender();
	void			Render();
	void			PostRender();
	void			Kill();

	// Transformation
	void			Translate(const D3DXVECTOR3& axis);
	void			Rotate(const D3DXVECTOR3& axis, float angle);
	void			Scale(const D3DXVECTOR3& axis);
	void			Update(float);
	void			SetShaderVariables();

	D3DXVECTOR4*	GetWorldPosition();

	inline D3DXMATRIX GetWorldMatrix() { return matWorld; }

private:
	LPCSTR			pGeomFileName;
	LPD3DXBUFFER	pAdjacency;
	LPD3DXBUFFER	pMaterials;
	LPD3DXBUFFER	pEffectInstances;
	DWORD			dwNumMaterials;
	LPD3DXMESH		pMesh;
	std::vector<D3DHelper::Mtrl> mtrls;
	std::vector<LPDIRECT3DTEXTURE9> tex;

	D3DXHANDLE		m_handleMtrl;
	D3DXHANDLE		m_handleTex;
	LPDIRECT3DTEXTURE9	mDefaultTex;
	LPDIRECT3DCUBETEXTURE9  mEnvmapTex;
	
	FxShader*		pFxShader;
	
	// Transformation
	D3DXMATRIX		matWorld;
	D3DXMATRIX		matTranslate;
	D3DXMATRIX		matRotation;
	D3DXMATRIX		matScale;

	Helper::StaticGeomData*	m_statGeomData;	
};

#endif

