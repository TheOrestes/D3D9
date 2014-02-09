
#include "Grass.h"
#include "../Misc/Helper.h"
#include "../Misc/LogManager.h"
#include "../Game/Camera.h"
#include "../Engine/Clock.h"
#include "../Renderables/BruteForceTerrain.h"
#include "../Misc/EngineStats.h"
#include "../RenderDevice/RenderDevice.h"

//------------------------------------------------------------------------
// Purpose  : GLOBALS
//------------------------------------------------------------------------
const int		NUM_GRASS_FINS = 12000;
const float		EPSILON  = 0.001f;

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Grass::Grass()
{
	pGrassVB	=	NULL;
	pGrassIB	=	NULL;
	pGrassTex	=	NULL;
	pEffect		=	NULL;
	pGrassMesh	=	NULL;
	pTerrain	=	NULL;

	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mProjection);
	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mWorldViewProjection);
	D3DXMatrixIdentity(&mViewProjection);	

	fTime		=	0.0f;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Grass::~Grass()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Initialize Grass Shaders
//------------------------------------------------------------------------
bool Grass::InitializeGrass(const std::string& textureName, BruteForceTerrain* ptrTerrain)
{
	// We can have different Grass mesh assigned to different Terrain type
	// Assign appropriate terrain mesh for this Grass!
	pTerrain = ptrTerrain;

	LPDIRECT3DDEVICE9	pDevice = RenderDevice::GetInstance()->getDevice();
	LPD3DXBUFFER	pErrBuffer = NULL;

	HRESULT Hr = D3DXCreateEffectFromFile(pDevice, "Shaders\\FX\\Grass.fx", NULL, NULL, 0, 0, &pEffect, &pErrBuffer);
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("Grass.fx ==> D3DXCreateEffectFromFile() failed!", (LPCSTR)pErrBuffer->GetBufferPointer());
		return false;
	}
	else
	{
		LogManager::GetInstance()->WriteToFile("Grass.fx Shader Effect Created!");
	}

	// Load Texture
	D3DXCreateTextureFromFile(pDevice, "Data\\Foliage\\grassfin0.dds", &pGrassTex);

	hMatWVP		= pEffect->GetParameterByName(0, "matWVP");
	hTechnique	= pEffect->GetTechniqueByName("GrassTech");
	hDelta		= pEffect->GetParameterByName(0, "delta");
	hTime		= pEffect->GetParameterByName(0, "gTime");
	hGrassTex0	= pEffect->GetParameterByName(0, "gTex");               
	hMatViewProj = pEffect->GetParameterByName(0, "gViewProj");
	hEyePos		= pEffect->GetParameterByName(0, "gEyePosW");

	pEffect->SetTechnique(hTechnique);
	pEffect->SetTexture(hGrassTex0, pGrassTex);

	// BUILD THE FIN!
	if(!BuildGrass())
		return false;
	else
	{
		UpdateEngineStats();
		return true;
	}
}

//------------------------------------------------------------------------
// Purpose  : Build the single Grass Fin!
//------------------------------------------------------------------------
void Grass::BuildGrassFin( GrassVertex* v, WORD* k, int& indexOffset, D3DXVECTOR3& worldPos, D3DXVECTOR3& scale )
{
	float amp = Helper::GetRandomFloat(0.2f, 1.0f);

	// set the vertices, top vertices move whereas bottom ones are fixed to ground
	v[0] = GrassVertex(D3DXVECTOR3(-1.0f, -0.5f, 0.0f), D3DXVECTOR2(0.0f, 1.0f), 0.0f);
	v[1] = GrassVertex(D3DXVECTOR3(-1.0f, 0.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), amp);
	v[2] = GrassVertex(D3DXVECTOR3(1.0f, 0.5f, 0.0f), D3DXVECTOR2(1.0f, 0.0f), amp);
	v[3] = GrassVertex(D3DXVECTOR3(1.0f, -0.5f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), 0.0f);
	
	// set the indices
	k[0] = 0 + indexOffset;
	k[1] = 1 + indexOffset;
	k[2] = 2 + indexOffset;
	k[3] = 0 + indexOffset;
	k[4] = 2 + indexOffset;
	k[5] = 3 + indexOffset;

	indexOffset += 4;

	// scale the fins & randomize green color
	for (int i = 0; i<4 ; ++i)
	{
		v[i].pos.x *= scale.x;
		v[i].pos.y *= scale.y;
		v[i].pos.z *= scale.z;

		// generate randomized green color
		v[i].colorOffset = D3DXCOLOR(
			Helper::GetRandomFloat(0.1f, 0.5f),
			Helper::GetRandomFloat(0.1f, 0.5f),
			Helper::GetRandomFloat(0.1f, 0.2f),
			0.0f);
	}

	// Add offset so that the bottom of fin touches the ground
	// when placed on terrain.  Otherwise, the fin's center point
	// will touch the ground and only half of the fin will show.
	float heightOver2 = (v[1].pos.y - v[0].pos.y) / 2;
	worldPos.y += heightOver2;

	// set the world position for the Quad
	v[0].quadPos = worldPos;
	v[1].quadPos = worldPos;
	v[2].quadPos = worldPos;
	v[3].quadPos = worldPos;
}

//------------------------------------------------------------------------
// Purpose  : Build Grass
//------------------------------------------------------------------------
bool Grass::BuildGrass()
{
	LPDIRECT3DDEVICE9	pDevice = RenderDevice::GetInstance()->getDevice();

	// Get vertex declaration
	D3DVERTEXELEMENT9	elems[MAX_FVF_DECL_SIZE];
	UINT numElems = 0;
	GrassVertex::pVertDecl->GetDeclaration(elems, &numElems);

	// Create total grass as a mesh
	HRESULT	Hr = D3DXCreateMesh(NUM_GRASS_FINS*2, NUM_GRASS_FINS*4, D3DXMESH_MANAGED, elems, pDevice, &pGrassMesh);
	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("ERROR ----> BuildGrass() D3DXCreateMesh FAILED!");
		return false;
	}

	GrassVertex* v = NULL;
	WORD* k = NULL;

	pGrassMesh->LockVertexBuffer(0, (void**)&v);
	pGrassMesh->LockIndexBuffer(0, (void**)&k);

	int indexOffset = 0;

	// scale down the region in which to generate the grass!
	int w = (int)(pTerrain->getWidth()* 1.0f);	// Do not multiply with anything
	int d = (int)(pTerrain->getDepth()* 1.0f);	// if full terrain grass is required!
	
	// randomize generation of Grass...
	for (int i = 0 ; i<NUM_GRASS_FINS ; ++i)
	{
		float x = (float)(rand()%w - (w*0.5f));
		float z = (float)(rand()%d - (d*0.5f));
		float y = pTerrain->getHeight(x,z);

		float sx = Helper::GetRandomFloat(5.25f, 7.25f);
		float sy = Helper::GetRandomFloat(8.5f, 9.5f);
		float sz = Helper::GetRandomFloat(5.75f, 7.25f);
		D3DXVECTOR3 pos(x, y, z);
		D3DXVECTOR3 scale(sx, sy, sz);

		BuildGrassFin(v, k, indexOffset, pos, scale);

		v += 4;
		k += 6;
	}

	pGrassMesh->UnlockVertexBuffer();
	pGrassMesh->UnlockIndexBuffer();

	// Fill in the attribute buffer ( everything in subset 0 )
	DWORD* attributeBuffer = 0;
	pGrassMesh->LockAttributeBuffer(0, &attributeBuffer);
	for (UINT i = 0 ; i<pGrassMesh->GetNumFaces() ; ++i)
	{
		attributeBuffer[i] = 0;
	}
	pGrassMesh->UnlockAttributeBuffer();

	DWORD*	adj = new DWORD[pGrassMesh->GetNumFaces()*3];
	pGrassMesh->GenerateAdjacency(EPSILON, adj);
	pGrassMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE, adj, 0, 0, 0);

	delete[] adj;

	LogManager::GetInstance()->WriteToFile("Grass Mesh Created...");
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Update Engine Stats
//------------------------------------------------------------------------
void Grass::UpdateEngineStats()
{
	DWORD	_vert = pGrassMesh->GetNumVertices();
	EngineStats::GetInstance()->addVertices(_vert);

	DWORD	_tris = pGrassMesh->GetNumFaces();
	EngineStats::GetInstance()->addTriangles(_tris);
}

//------------------------------------------------------------------------
// Purpose  : Render
//------------------------------------------------------------------------
void Grass::RenderGrass()
{
	LPDIRECT3DDEVICE9	pDevice = RenderDevice::GetInstance()->getDevice();

	D3DXVECTOR4 camPos = Camera::GetInstance()->m_Position;

	// Shader Stuff happens here...
	pEffect->SetFloat(hDelta, delta);
	pEffect->SetFloat(hTime, fTime);
	pEffect->SetMatrix(hMatViewProj, &mViewProjection);
	pEffect->SetVector(hEyePos, &camPos);
	pEffect->CommitChanges();

	// Begin Passes
	UINT nPasses = 0;
	pEffect->Begin(&nPasses, 0);
	pEffect->BeginPass(0);
	pGrassMesh->DrawSubset(0);		
	pEffect->EndPass();
	pEffect->End();
}

//------------------------------------------------------------------------
// Purpose  : Update the foliage
//------------------------------------------------------------------------
void Grass::UpdateGrass(float dt)
{
	delta = dt;
	fTime += dt;

	D3DXMATRIX* matView = RenderDevice::GetInstance()->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = RenderDevice::GetInstance()->GetGlobalProjectionMatrix();

	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	mViewProjection = matViewVal * matProjVal;
}

//------------------------------------------------------------------------
// Purpose  : Kill Everything!
//------------------------------------------------------------------------
void Grass::Kill()
{
	SAFE_RELEASE(pGrassVB);
	SAFE_RELEASE(pGrassIB);
	SAFE_RELEASE(pGrassTex);
	SAFE_RELEASE(pEffect);
	SAFE_RELEASE(pGrassMesh);
	LogManager::GetInstance()->WriteToFile("Grass Shutdown...");
}


