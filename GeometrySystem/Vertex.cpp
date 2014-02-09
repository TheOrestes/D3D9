
#include "Vertex.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : Initialize Static variables
//------------------------------------------------------------------------
LPDIRECT3DVERTEXDECLARATION9	VertexPos::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexCol::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexPN::pVertDecl	 = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexPT::pVertDecl	 = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexPNTZ::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexPNTBT::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	VertexPP::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	Particle::pVertDecl = NULL;
LPDIRECT3DVERTEXDECLARATION9	GrassVertex::pVertDecl	= NULL;

//------------------------------------------------------------------------
// Purpose  : Initialize All Vertex Declarations
//------------------------------------------------------------------------
void	InitAllVertexDeclarations()
{
	// Get rendering device
	LPDIRECT3DDEVICE9	gDevice = RenderDevice::GetInstance()->getDevice();

	//////////////////////////////////////////////////////////////////////////
	// VertexPos

	D3DVERTEXELEMENT9	VertexPosElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPosElements, &VertexPos::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPos Vertex Declaration Failed!",__FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexCol

	D3DVERTEXELEMENT9	VertexColElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexColElements, &VertexCol::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexCol Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexPN

	D3DVERTEXELEMENT9	VertexPNElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPNElements, &VertexPN::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPN Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexPT

	D3DVERTEXELEMENT9	VertexPTElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPTElements, &VertexPT::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPT Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexPNT

	D3DVERTEXELEMENT9	VertexPNTElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPNTElements, &VertexPNTZ::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPNT Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexPNTBT

	D3DVERTEXELEMENT9	VertexPNTBTElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
		{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
		{0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPNTBTElements, &VertexPNTBT::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPNTBT Vertex Declaration Failed!",__FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// VertexPP

	D3DVERTEXELEMENT9	VertexPPElements[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexPPElements, &VertexPP::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("VertexPP Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Particle
	D3DVERTEXELEMENT9	VertexParticleElems[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
		{0, 36, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
		{0, 40, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexParticleElems, &Particle::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("Particle Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Grass related
	D3DVERTEXELEMENT9	VertexGrassElems[] = 
	{
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		{0, 36, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};

	if(S_OK != gDevice->CreateVertexDeclaration(VertexGrassElems, &GrassVertex::pVertDecl))
	{
		LogManager::GetInstance()->WriteToFile("Grass Vertex Declaration Failed!", __FILE__, __LINE__);
		return;
	}

	LogManager::GetInstance()->WriteToFile("Vertex Declaration Successful");
}

//------------------------------------------------------------------------
// Purpose  : Destroy All Vertex Declarations
//------------------------------------------------------------------------
void	DestroyAllVertexDeclarations()
{
	SAFE_RELEASE(VertexPos::pVertDecl);
	SAFE_RELEASE(VertexCol::pVertDecl);
	SAFE_RELEASE(VertexPN::pVertDecl);
	SAFE_RELEASE(VertexPT::pVertDecl);
	SAFE_RELEASE(VertexPP::pVertDecl);
	SAFE_RELEASE(VertexPNTZ::pVertDecl);
	SAFE_RELEASE(VertexPNTBT::pVertDecl);
	SAFE_RELEASE(Particle::pVertDecl);
	SAFE_RELEASE(GrassVertex::pVertDecl);

	LogManager::GetInstance()->WriteToFile("Vertex Declarations successfully destroyed");
}
