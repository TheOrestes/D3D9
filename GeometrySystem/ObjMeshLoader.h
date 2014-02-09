#ifndef _OBJMESH_LOADER_H_
#define _OBJMESH_LOADER_H_

#include <iomanip>
#include <d3dx9.h>
#include <string>
#include <fstream>
#include <vector>

struct	VERTEX
{
	VERTEX():position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), texcoord(0.0f, 0.0f){}
	VERTEX(float x , float y, float z, float nx, float ny, float nz, float u, float v)
			:position(x,y,z), normal(nx,ny,nz), texcoord(u,v){}
	VERTEX(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
			:position(v), normal(n), texcoord(uv){}

	D3DXVECTOR3	position;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	texcoord;
};

struct	CacheEntry
{
	UINT index;
	CacheEntry*	pNext;
};

struct Material
{
	std::string strName;

	D3DXVECTOR3			vAmbient;
	D3DXVECTOR3			vDiffuse;
	D3DXVECTOR3			vSpecular;

	int					nShininess;
	float				fAlpha;
	bool				bSpecular;

	std::string			strTexture;
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXHANDLE			hTechnique;
};

class ObjMeshLoader
{
public:
	ObjMeshLoader();
	~ObjMeshLoader();

	bool			Init(const std::string& fileName);
	bool			LoadGeometryFromOBJ(const std::string& fileName);
	void			RenderSubset(UINT index);
	void			Update(float dt);
	void			Translate(const D3DXVECTOR3& axis);
	void			Rotate(const D3DXVECTOR3& axis, float angle);
	void			Scale(const D3DXVECTOR3& axis);
	void			Kill();

	UINT    GetNumMaterials() const
	{
		return m_Materials.size();
	}

	Material* GetMaterial( UINT iMaterial )
	{
		return m_Materials.at( iMaterial );
	}

	ID3DXMesh* GetMesh()
	{
		return m_pMesh;
	}

private:
	HRESULT			LoadMaterialsFromMTL(const std::string& fileName);
	void			InitMaterial(Material&	pMaterial);

	DWORD			AddVertex(UINT hash, VERTEX& pVertex);
	void			DeleteCache();

	LPD3DXMESH		m_pMesh;
	LPD3DXEFFECT	m_pEffect;
	D3DXHANDLE      g_hAmbient; 
	D3DXHANDLE      g_hDiffuse;
	D3DXHANDLE      g_hSpecular;
	D3DXHANDLE      g_hOpacity;
	D3DXHANDLE      g_hSpecularPower;
	D3DXHANDLE      g_hLightColor;
	D3DXHANDLE      g_hLightPosition;
	D3DXHANDLE      g_hCameraPosition;
	D3DXHANDLE      g_hTexture;
	D3DXHANDLE      g_hTime;
	D3DXHANDLE      g_hWorld;
	D3DXHANDLE      g_hWorldViewProjection;

	std::vector<CacheEntry*>	m_VertexCache;
	std::vector<VERTEX>		m_Vertices;
	std::vector<DWORD>		m_Indices;
	std::vector<DWORD>		m_Attributes;
	std::vector<Material*>	m_Materials;

	D3DXMATRIX		matWorld;
	D3DXMATRIX		matTranslate;
	D3DXMATRIX		matRotation;
	D3DXMATRIX		matScale;

	std::string				m_strMediaDir;
};

#endif