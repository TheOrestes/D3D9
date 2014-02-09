// ***************************************************************
//  BRUTE_FORCE_TERRAIN			
//  -------------------------------------------------------------
//  Purpose : Render Terrain using Brute Force Method	
// ***************************************************************

#ifndef BRUTE_FORCE_TERRAIN
#define BRUTE_FORCE_TERRAIN

#include <d3dx9.h>
#include <vector>

// Terrain Vertex
struct TerrainVertex
{
	TerrainVertex();
	TerrainVertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
	{
		x = _x;		y = _y;		z = _z;		nx = _nx;	ny = _ny;	nz = _nz;	u = _u;		v = _v;
	}

	float x, y, z, nx, ny, nz, u, v;
};

class BruteForceTerrain
{
public:
	BruteForceTerrain(int _nVertPerRow, int _nColsPerRow, int _nCellSpacing, float _nHeightScale);
	virtual ~BruteForceTerrain();

	// HeightMap related functions
	bool	ReadRAWFIle(const std::string& filename);
	
	// Initialization calls
	bool	InitTerrain();
	bool	CreateVertices();
	bool	CreateIndices();
	bool	InitMixMapShader(const std::string& filename);

	// Texture calls
	bool	LoadTextures();
	bool	GenerateTexture();
	bool	LightTerrain(const D3DXVECTOR3& direction);
	D3DXVECTOR3* ComputeNormal(int cellrow, int cellCol);
	
	// Rendering calls
	float	ComputeShade(int cellrow, int cellCol, const D3DXVECTOR3& direction);
	void	preRender();
	void	Render();
	void	postRender();
	void	Update();

	// ENGINE CALLS
	void	UpdateEngineStats();

	// INLINE Calls
	int		getHeightmapEntry(int row, int col);
	void	setHeightmapEntry(int row, int col, int val);

	// HELPER Functions
	int		getWidth(){ return nWidth; }
	int		getDepth(){ return nDepth; }
	float	getHeight(float x, float z);

	void	Kill();

private:
	TerrainVertex*				m_pTerrainVertex;
	IDirect3DVertexBuffer9*		m_pVB;
	IDirect3DIndexBuffer9*		m_pIB;
	IDirect3DTexture9*			m_pTerrTex;
	IDirect3DTexture9*			m_pGenTex;

	std::vector<int>			heightmap;

	// Terrain Properties
	int		nVertPerRow;
	int		nVertPerCol;
	int		nCellSpacing;

	int		nCellsPerRow;
	int		nCellsPerCol;
	int		nWidth;
	int		nDepth;
	int		nVertices;
	int		nTriangles;

	float	nHeightScale;

	D3DXMATRIX	matWorld;
	D3DXMATRIX	matTrans;
	D3DXMATRIX	matRot;
	D3DXMATRIX	matScale;
	D3DXMATRIX	matWVP;

	// Mix-map Shader Stuff
	IDirect3DTexture9*		m_pTex0;
	IDirect3DTexture9*		m_pTex1;
	IDirect3DTexture9*		m_pTex2;
	IDirect3DTexture9*		m_pBlendMap;

	D3DXVECTOR4				m_LightDir;

	ID3DXEffect*			m_pFX;
	D3DXHANDLE				m_hTech;
	D3DXHANDLE				m_hTex0;
	D3DXHANDLE				m_hTex1;
	D3DXHANDLE				m_hTex2;
	D3DXHANDLE				m_hBlendMap;
	D3DXHANDLE				m_hMatWVP;
	D3DXHANDLE				m_hMatWorld;
	D3DXHANDLE				m_hLightDir;
	D3DXHANDLE				m_hEyePos;
};

#endif