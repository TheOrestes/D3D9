
#include "BruteForceTerrain.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/EngineStats.h"
#include "../Scene/Scene.h"
#include "../Misc/Helper.h"

#define TERRAIN_SHADOW 

//------------------------------------------------------------------------
// Purpose  : GLOBALS!
//------------------------------------------------------------------------
LogManager*	Log = LogManager::GetInstance();

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
BruteForceTerrain::BruteForceTerrain(int _nVertPerRow, int _nColsPerRow, 
									 int _nCellSpacing, float _nHeightScale)
{
	m_pTerrainVertex	=	NULL;
	m_pVB				=	NULL;
	m_pTerrTex			=	NULL;
	m_pGenTex			=	NULL;

	nVertPerRow			=	_nVertPerRow;
	nVertPerCol			=	_nColsPerRow;
	nCellSpacing		=	_nCellSpacing;
	nHeightScale		=	_nHeightScale;

	// Get the basic calculation going...
	nCellsPerRow	=	nVertPerRow - 1;
	nCellsPerCol	=	nVertPerCol - 1;
	nWidth			=	nCellsPerRow * nCellSpacing;
	nDepth			=	nCellsPerCol * nCellSpacing;
	nVertices		=	nVertPerRow * nVertPerCol;
	nTriangles		=	nCellsPerRow * nCellsPerCol * 2;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matWVP);
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
BruteForceTerrain::~BruteForceTerrain()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Read the RAW file 
//------------------------------------------------------------------------
bool	BruteForceTerrain::ReadRAWFIle(const std::string& filename)
{
	std::ifstream inFile(filename.c_str(), std::ios_base::binary);

	if (inFile == 0)
	{
		Log->WriteToFile("RAW file load fail");
		return false;
	}

	// [6/24/2012 DoD]
	// Find out how many bytes of data exists in the RAW file
	inFile.seekg (0, ios::end);
	int length = inFile.tellg();

	// Set the reading position back to beginning!
	inFile.seekg (0, ios::beg);	

	// A height for each vertex
	std::vector<BYTE> _in (length);

	inFile.read((char*)&_in[0],				// buffer
				length);					// no. of bytes to read

	inFile.close();

	// Copy BYTE vector to int vector
	heightmap.resize(length);
	for (int i = 0 ; i< length ; ++i)
	{
		heightmap[i] = _in[i] * nHeightScale;
	}

	Log->WriteToFile("RAW File read Successful");
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Access Heightmap
//------------------------------------------------------------------------
inline int BruteForceTerrain::getHeightmapEntry(int row, int col)
{
	// Original : return heightmap[row*nVertPerRow + col];
	// Date     : 24/6/2012
	// function used to crash when negative row value was being passed,
	// as the effective heightmap lookup was for a negative index, making
	// it fabs() makes sure we always look up in positive direction
	// Don't know the side effects as of now! 
	//return heightmap[fabs(float(row))*nVertPerRow + col];

	// Below is the life saving hack! Index goes above heighmap[MAX], hence
	// the code crashes!
	if (row >= nVertPerRow)
	{
		--row;
	}

	if (col >= nVertPerCol)
	{
		--col;
	}

	return heightmap[row*nVertPerRow + col];
}

//------------------------------------------------------------------------
// Purpose  : Modify Heightmap
//------------------------------------------------------------------------
inline void	BruteForceTerrain::setHeightmapEntry(int row, int col, int val)
{
	heightmap[row*nVertPerRow + col] = val;
}

//------------------------------------------------------------------------
// Purpose  : Get height at Point on Terrain
//------------------------------------------------------------------------
float	BruteForceTerrain::getHeight(float x, float z)
{
	float height;

	// take terrain start point at the origin 
	x = ((float)nWidth / 2.0f) + x;
	z = ((float)nDepth / 2.0f) - z;		// flip Z axis

	// Scale down cellSpacing by one
	x /= (float)nCellSpacing;
	z /= (float)nCellSpacing;

	// get the row & column of the cell we are in
	float col = floorf(x);
	float row = floorf(z);

	// Now we know the cell we are in, grab the heights of 4 vertices 
	// A	B
	// *----*
	// |   /|
	// | /  |
	// *----*
	// C	D

	float A = getHeightmapEntry(row, col);
	float B = getHeightmapEntry(row, col+1);
	float C = getHeightmapEntry(row+1, col);
	float D = getHeightmapEntry(row+1, col+1);

	// In order to find the height, we need to know which triangle of the Quad 
	// we are in, since Quad is rendered as 2 triangles, we'll take a quad we 
	// are in, & translate it so that its upper left vertex is at Origin.
	// i.e. -row & -col

	float dx = x - col;
	float dz = z - row;

	if (dz < 1.0f - dx)		// upper triangle ABC
	{
		float uy = B - A;	// A->B
		float vy = C - A;	// A->C

		height = A + Helper::Lerp(0.0f, uy, dx) + Helper::Lerp(0.0f, vy, dz);
	}
	else					// lower triangle DCB
	{
		float uy = C -D;	// D->C
		float vy = B -D;	// D->B

		height = D + Helper::Lerp(0.0f, uy, 1.0f-dx) + 
					 Helper::Lerp(0.0f, vy, 1.0-dz);
	}

	return height;// * nHeightScale;
}

//------------------------------------------------------------------------ 
// Purpose  : Initialize Terrain Geometry
//------------------------------------------------------------------------
bool	BruteForceTerrain::InitTerrain()
{
	
	// Create Vertices Here...
	if (!CreateVertices())
	{
		Log->WriteToFile("Terrain Vertex Creation Failed!");
		return false;
	}

	// Create Indices Here
	if (!CreateIndices())
	{
		Log->WriteToFile("Terrain Index Creation Failed");
		return false;
	}

	// Texturing
	// if (!GenerateTexture())
 	// {
 	// 	Log->WriteToFile("Texture generation Failed");
 	// 	return false;
 	// }

	if (!LoadTextures())
	{
		Log->WriteToFile("Terrain texture loading Failed");
		return false;
	}

	InitMixMapShader("Shaders\\FX\\Terrain.fx");

	UpdateEngineStats();

	return true;
}

//------------------------------------------------------------------------ 
// Purpose  : Calculate Terrain Vertices
//------------------------------------------------------------------------
bool	BruteForceTerrain::CreateVertices()
{
	HRESULT Hr = 0;

	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	// Create Terrain Vertex BUffer
	Hr = device->CreateVertexBuffer(nVertices*sizeof(TerrainVertex), 
		D3DUSAGE_WRITEONLY, 
		D3DFVF_XYZ | D3DFVF_TEX1, 
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL);

	if (FAILED(Hr))
	{
		Log->WriteToFile("Failed to create Vertex Buffer for Terrain!");
		return false;
	}

	// start generating vertices here
	int startX = -nWidth / 2;
	int startZ = nDepth / 2;

	// end generating vertices here
	int endX = nWidth / 2;
	int endZ = -nDepth / 2;

	// Compute increment in tex coords here
	float uCoordIncSize = 1.0f/(float)nCellsPerRow;
	float vCoordIncSize = 1.0f/(float)nCellsPerCol;

	m_pVB->Lock(0, 0, (void**)&m_pTerrainVertex, 0);

	int i = 0;
	for (int z = startZ ; z >= endZ ; z -= nCellSpacing)
	{
		int j = 0;
		for (int x = startX ; x <= endX; x += nCellSpacing)
		{
			// index calculation
			int index = i * nVertPerRow + j;

#if defined TERRAIN_SHADOW
	D3DXVECTOR3*	normal = ComputeNormal(i,j);
	m_pTerrainVertex[index] = TerrainVertex( 
											(float)x, 
											(float)heightmap[index], 
											(float)z, 
											normal->x, 
											normal->y, 
											normal->z, 
											(float)j * uCoordIncSize, 
											(float)i * vCoordIncSize );
#else
	m_pTerrainVertex[index] = TerrainVertex( 
											(float)x, 
											(float)heightmap[index], 
											(float)z, 
											0.0f, 
											1.0f, 
											0.0f, 
											(float)j * uCoordIncSize, 
											(float)i * vCoordIncSize );
#endif

			j++;	// next column
		}
		i++;	// next row
	}

	m_pVB->Unlock();

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Create Terrain Indices
//------------------------------------------------------------------------
bool	BruteForceTerrain::CreateIndices()
{
	HRESULT Hr = 0;

	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	Hr = device->CreateIndexBuffer(	nTriangles*3*sizeof(WORD),
									D3DUSAGE_WRITEONLY,
									D3DFMT_INDEX16,
									D3DPOOL_MANAGED,
									&m_pIB,
									NULL);
	if (FAILED(Hr))
	{
		Log->WriteToFile("Failed to create Index Buffer for Terrain!");
		return false;
	}

	WORD*	index = 0;

	m_pIB->Lock(0, 0, (void**)&index, 0);

	int baseIndex = 0;

	for (int i = 0 ; i < nCellsPerCol ; i++)
	{
		for (int j = 0 ; j < nCellsPerRow ; j++)
		{
			index[baseIndex]	 = i * nVertPerRow + j;
			index[baseIndex + 1] = i * nVertPerRow + j + 1;
			index[baseIndex + 2] = (i+1) * nVertPerRow + j;
			index[baseIndex + 3] = (i+1) * nVertPerRow + j;
			index[baseIndex + 4] = i * nVertPerRow + j + 1;
			index[baseIndex + 5] = (i+1) * nVertPerRow + j + 1;
		
			// next Quad
			baseIndex += 6;
		}
	}

	m_pIB->Unlock();

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Load Texture
//------------------------------------------------------------------------
bool	BruteForceTerrain::LoadTextures()
{
	HRESULT Hr;

	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	if (S_OK != D3DXCreateTextureFromFile(device, "Data\\Terrain\\Grass-texture.jpg", &m_pTex0))
	{
		Log->WriteToFile("Data\\Terrain\\grass.dds", "loading FAILED!");
		return false;
	}

	if (S_OK != D3DXCreateTextureFromFile(device, "Data\\Terrain\\grass_dried.jpg", &m_pTex1))
	{
		Log->WriteToFile("Data\\Terrain\\dirt.dds", "loading FAILED!");
		return false;
	}

	if (S_OK != D3DXCreateTextureFromFile(device, "Data\\Terrain\\dirt.png", &m_pTex2))
	{
		Log->WriteToFile("Data\\Terrain\\stone.dds", "loading FAILED!");
		return false;
	}

	if (S_OK != D3DXCreateTextureFromFile(device, "Data\\Terrain\\best_mix.png", &m_pBlendMap))
	{
		Log->WriteToFile("Data\\Terrain\\best_mix.png", "loading FAILED!");
		return false;
	}

	D3DXVECTOR4 LightDirection = Scene::GetInstance()->m_GlobalLightDirection;
	m_LightDir = LightDirection;

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Initialize Mixmap Shader
//------------------------------------------------------------------------
bool	BruteForceTerrain::InitMixMapShader(const std::string& filename)
{
	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	ID3DXBuffer*	errBuffer = 0;

	D3DXCreateEffectFromFile(device, filename.c_str(), 0, 0, D3DXSHADER_DEBUG, 0, &m_pFX, &errBuffer);

	if (errBuffer)
	{
		LogManager::GetInstance()->WriteToFile("ERROR -----> ", (char*)errBuffer->GetBufferPointer());
		return false;
	}
	else
	{
		LogManager::GetInstance()->WriteToFile(filename.c_str(), " Effect Created..." );
	}

	// Get handles
	m_hTech		=	m_pFX->GetTechniqueByName("Terrain");
	m_hTex0		=	m_pFX->GetParameterByName(0, "gTex0");
	m_hTex1		=	m_pFX->GetParameterByName(0, "gTex1");
	m_hTex2		=	m_pFX->GetParameterByName(0, "gTex2");
	m_hBlendMap	=	m_pFX->GetParameterByName(0, "gBlendMap");
	m_hMatWVP	=	m_pFX->GetParameterByName(0, "gMatWVP");
	m_hMatWorld	=	m_pFX->GetParameterByName(0, "gMatWorld");
	m_hLightDir	=	m_pFX->GetParameterByName(0, "gLightDir");
	m_hEyePos	=	m_pFX->GetParameterByName(0, "gEyePos");

	m_pFX->SetTexture(m_hTex0, m_pTex0);
	m_pFX->SetTexture(m_hTex1, m_pTex1);
	m_pFX->SetTexture(m_hTex2, m_pTex2);
	m_pFX->SetTexture(m_hBlendMap, m_pBlendMap);
}

//------------------------------------------------------------------------
// Purpose  : Generate Terrain texture based on HeightMap
//------------------------------------------------------------------------
bool	BruteForceTerrain::GenerateTexture()
{
	HRESULT Hr;

	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	int texWidth	= nCellsPerRow;
	int texHeight	= nCellsPerCol;

	// Create an empty texture
	Hr = D3DXCreateTexture(device, texWidth, texHeight, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pGenTex);

	if (FAILED(Hr))
		return false;

	// Reverify the surface info
	D3DSURFACE_DESC texDesc;
	m_pGenTex->GetLevelDesc(0, &texDesc);
	if (texDesc.Format != D3DFMT_A8R8G8B8)
		return false;

	// Fill the texture
	D3DLOCKED_RECT	lockRect;
	m_pGenTex->LockRect(0, &lockRect, 0, 0);

	DWORD*	imageData = (DWORD*)lockRect.pBits;

	for (int i = 0 ; i<texHeight ; i++)
	{
		for (int j = 0 ; j<texWidth ; j++)
		{
			D3DCOLOR c;
			float height = (float)getHeightmapEntry(i,j)/nHeightScale;

			// Set color based on height
			// LAMEST POSSIBLE CHOICE, NEED TO DO BETTER!
			if (height < 42.5f)
				c = D3DCOLOR_XRGB(141, 98, 18);
			else if(height < 50.0f)
				c = D3DCOLOR_XRGB(141, 101, 18);
			else if(height < 65.0f)
				c = D3DCOLOR_XRGB(141, 113, 18);
			else if(height < 85.0f)
				c = D3DCOLOR_XRGB(141, 124, 18);
			else if(height < 100.0f)
				c = D3DCOLOR_XRGB(141, 132, 18);
			else if(height < 120.0f)
				c = D3DCOLOR_XRGB(141, 139, 18);
			else if(height < 135.0f)
				c = D3DCOLOR_XRGB(141, 148, 18);
			else if(height < 160.0f)
				c = D3DCOLOR_XRGB(141, 154, 18);
			else if(height < 175.0f)
				c = D3DCOLOR_XRGB(141, 167, 18);
			else if(height < 190.0f)
				c = D3DCOLOR_XRGB(141, 176, 18);
			else if(height < 200.0f)
				c = D3DCOLOR_XRGB(141, 185, 18);
			else if(height < 215.0f)
				c = D3DCOLOR_XRGB(141, 191, 18);
			else if(height < 235.0f)
				c = D3DCOLOR_XRGB(141, 199, 18);
			else if(height < 245.5f)
				c = D3DCOLOR_XRGB(141, 210, 18);
			else if(height < 248.5f)
				c = D3DCOLOR_XRGB(141, 225, 18);
			else if(height < 250.5f)
				c = D3DCOLOR_XRGB(141, 237, 18);
			else
				c = D3DCOLOR_XRGB(141, 238, 18);

			//c *= D3DCOLOR_XRGB((int)height, (int)height, (int)height);

			// We divide Pitch by 4, since the pitch is given in bytes & there are
			// 4 bytes per DWORD
			imageData[i * lockRect.Pitch/4 + j] = c;
		}
	}

	m_pGenTex->UnlockRect(0);

	// Light the Terrain
	//D3DXVECTOR3 sunDirection(0.0f, 1.0f, 0.0f);
	//if (!LightTerrain(sunDirection))
	//	return false;

	//// Fill Mipmaps
	//Hr = D3DXFilterTexture(m_pGenTex, 0, 0, D3DX_DEFAULT);

	//if (FAILED(Hr))
	//	return false;

	return true;
}

//------------------------------------------------------------------------ 
// Purpose  : Compute Shade of each Quad
//------------------------------------------------------------------------
D3DXVECTOR3*	BruteForceTerrain::ComputeNormal(int cellrow, int cellCol)
{
	// get heights of three vertices on the Quad
	float heightA	=	getHeightmapEntry(cellrow, cellCol);
	float heightB	=	getHeightmapEntry(cellrow, cellCol+1);
	float heightC	=	getHeightmapEntry(cellrow+1, cellCol);

	// build two vectors on the Quad
	D3DXVECTOR3 u(nCellSpacing, heightB-heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC-heightA, -nCellSpacing);

	// surface normal
	D3DXVECTOR3	n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	return &n;
}

//------------------------------------------------------------------------
// Purpose  : Update Engine Stats
//------------------------------------------------------------------------
void	BruteForceTerrain::UpdateEngineStats()
{
	DWORD	_vert = (DWORD)nVertices;
	EngineStats::GetInstance()->addVertices(_vert);

	DWORD	_tris = (DWORD)nTriangles;
	EngineStats::GetInstance()->addTriangles(_tris);
}

//------------------------------------------------------------------------
// Purpose  : Compute Shade of each Quad
//------------------------------------------------------------------------
float	BruteForceTerrain::ComputeShade(int cellrow, int cellCol, const D3DXVECTOR3& direction)
{
	// get heights of three vertices on the Quad
	float heightA	=	getHeightmapEntry(cellrow, cellCol);
	float heightB	=	getHeightmapEntry(cellrow, cellCol+1);
	float heightC	=	getHeightmapEntry(cellrow+1, cellCol);

	// build two vectors on the Quad
	D3DXVECTOR3 u(nCellSpacing, heightB-heightA, 0.0f);
	D3DXVECTOR3 v(0.0f, heightC-heightA, -nCellSpacing);

	// surface normal
	D3DXVECTOR3	n;
	D3DXVec3Cross(&n, &u, &v);
	D3DXVec3Normalize(&n, &n);

	// Shader factor
	float cosine = D3DXVec3Dot(&n, &direction);

	cosine = cosine * 0.5f + 0.5f;

	if (cosine<0.0f)
		cosine = 0.0f;

	return cosine;
}

//------------------------------------------------------------------------ 
// Purpose  : Light the Terrain ourselves!
//------------------------------------------------------------------------
bool	BruteForceTerrain::LightTerrain(const D3DXVECTOR3& direction)
{
	// Get surface description
	D3DSURFACE_DESC texDesc;
	m_pGenTex->GetLevelDesc(0, &texDesc);

	// lock the desired area
	D3DLOCKED_RECT	lockRect;
	m_pGenTex->LockRect(0, &lockRect, 0, 0);

	DWORD* imageData = (DWORD*)lockRect.pBits;

	for(int i = 0 ; i < texDesc.Height-1 ; i++)
	{
		for (int j = 0 ; j< texDesc.Width-1 ; j++)
		{
			int index = i * lockRect.Pitch / 4 + j;

			// get current color of a cell
			D3DXCOLOR c(imageData[index]);

			// shader current cell
			c *= ComputeShade(i, j, direction);
			
			// save shaded color
			imageData[index] = (D3DCOLOR)c;
		}
	}

	return true;
}

//------------------------------------------------------------------------  
// Purpose  : PreRender Terrain
//------------------------------------------------------------------------
void	BruteForceTerrain::preRender()
{
	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	// Set texture filters.
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//------------------------------------------------------------------------ 
// Purpose  : PostRender Terrain
//------------------------------------------------------------------------
void	BruteForceTerrain::postRender()
{
	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//------------------------------------------------------------------------
// Purpose  : Render using Brute Force approach
//------------------------------------------------------------------------
void	BruteForceTerrain::Render()
{
	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	RenderDevice* dev = RenderDevice::GetInstance();

	// Get Camera ViewProj Matrix
	D3DXMATRIX* matView = dev->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = dev->GetGlobalProjectionMatrix();

	// Create WorldViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	D3DXMATRIX wvp;

	wvp = matViewVal * matProjVal;

	// Set Global LightDirection
	m_pFX->SetVector(m_hLightDir, &m_LightDir);

	// Make WVP matrix & set
	m_pFX->SetMatrix(m_hMatWVP, &wvp);
	m_pFX->SetMatrix(m_hMatWorld, &matWorld);
	m_pFX->SetTechnique(m_hTech);
	UINT nPasses = 0;
	m_pFX->Begin(&nPasses, 0);
	m_pFX->BeginPass(0);

	device->SetStreamSource( 0, m_pVB,0, sizeof(TerrainVertex) );
	device->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	device->SetIndices(m_pIB);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nVertices, 0, nTriangles);	

	m_pFX->EndPass();
	m_pFX->End();
}

//------------------------------------------------------------------------
// Purpose  : Kill Terrain..err..i mean..remove terrain resources!
//------------------------------------------------------------------------
void	BruteForceTerrain::Kill()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);

	if (m_pTerrainVertex)
		m_pTerrainVertex = NULL;

	SAFE_RELEASE(m_pTerrTex);
	SAFE_RELEASE(m_pGenTex);

	LogManager::GetInstance()->WriteToFile("BruteForceTerrain Shutdown...");
}


