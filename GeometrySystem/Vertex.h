// ***************************************************************
//  Vertex			
//  -------------------------------------------------------------
//  Purpose :	All Vertex structure declarations
// ***************************************************************

#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>

// Fwd Declaration
void	InitAllVertexDeclarations();
void	DestroyAllVertexDeclarations();

//////////////////////////////////////////////////////////////////////////
struct VertexPos 
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x , float y, float z):pos(x,y,z){}
	VertexPos(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3	pos;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct VertexCol
{
	VertexCol():pos(0.0f, 0.0f, 0.0f), col(0x00000000){}
	VertexCol(float x, float y, float z, D3DCOLOR c):pos(x,y,z), col(c){}
	VertexCol(const D3DXVECTOR3& v, D3DCOLOR c):pos(v), col(c){}

	D3DXVECTOR3	pos;
	D3DCOLOR	col;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};
	
//////////////////////////////////////////////////////////////////////////
struct VertexPN
{
	VertexPN():pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f){}
	VertexPN(float x, float y, float z, float nx, float ny, float nz):pos(x,y,z), normal(nx,ny,nz){}
	VertexPN(const D3DXVECTOR3& v, const D3DXVECTOR3& n):pos(v), normal(n){}

	D3DXVECTOR3	pos;
	D3DXVECTOR3	normal;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct VertexPT
{
	VertexPT():pos(0.0f, 0.0f, 0.0f), tex0(0.0f, 0.0f){}
	VertexPT(float x, float y, float z, float u, float v):pos(x,y,z), tex0(u,v){}
	VertexPT(const D3DXVECTOR3& v, const D3DXVECTOR2& uv):pos(v), tex0(uv){}

	D3DXVECTOR3	pos;
	D3DXVECTOR2	tex0;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct VertexPNTZ
{
	VertexPNTZ():pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex0(0.0f, 0.0f){}
	VertexPNTZ(float x, float y, float z, float nx, float ny, float nz, float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
	VertexPNTZ(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv):pos(v), normal(n), tex0(uv){}

	D3DXVECTOR3	pos;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	tex0;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct VertexPNTBT
{
	VertexPNTBT():	pos(0.0f, 0.0f, 0.0f), 
					normal(0.0f, 0.0f, 0.0f), 
					binormal(0.0f, 0.0f, 0.0f), 
					tangent(0.0f, 0.0f, 0.0f), 
					tex0(0.0f, 0.0f){}

	VertexPNTBT(	float x, float y, float z, 
					float nx, float ny, float nz, 
					float bx, float by, float bz,
					float tx, float ty, float tz,  
					float u, float v)	:	pos(x,y,z), normal(nx,ny,nz), 
											binormal(bx, by, bz),
											tangent(tx, ty, tz), tex0(u,v){}

	VertexPNTBT(	const D3DXVECTOR3& v, 
					const D3DXVECTOR3& n, 
					const D3DXVECTOR3& t, 
					const D3DXVECTOR3& b, 
					const D3DXVECTOR2& uv):pos(v), normal(n), tangent(t), binormal(b),
					tex0(uv){}

	D3DXVECTOR3	pos;
	D3DXVECTOR3	normal;
	D3DXVECTOR3	binormal;
	D3DXVECTOR3	tangent;
	D3DXVECTOR2	tex0;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct VertexPP
{
	VertexPP():pos(0.0f, 0.0f, 0.0f, 1.0f), tex0(0.0f, 0.0f){}
	VertexPP(float x, float y, float z, float w, float u, float v):pos(x,y,z,w), tex0(u,v){}
	VertexPP(const D3DXVECTOR4& v, const D3DXVECTOR2& uv):pos(v), tex0(uv){}

	D3DXVECTOR4	pos;
	D3DXVECTOR2	tex0;
	static LPDIRECT3DVERTEXDECLARATION9	pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct Particle
{
	D3DXVECTOR3		initialPos;
	D3DXVECTOR3		initialVelocity;
	float			initialSize;
	float			initialTime;
	float			lifeTime;
	float			mass;
	D3DCOLOR		initialColor;
	static LPDIRECT3DVERTEXDECLARATION9 pVertDecl;
};

//////////////////////////////////////////////////////////////////////////
struct GrassVertex
{
	GrassVertex():
	pos(0.0f, 0.0f, 0.0f),
	tex0(0.0f, 0.0f),
	amplitude(0.0f){}

	GrassVertex(const D3DXVECTOR3& v,	
				const D3DXVECTOR2& uv,
				float amp) : pos(v), tex0(uv), amplitude(amp){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 quadPos;
	D3DXVECTOR2	tex0;
	float		amplitude;
	D3DCOLOR	colorOffset;
	static LPDIRECT3DVERTEXDECLARATION9 pVertDecl;
};

#endif