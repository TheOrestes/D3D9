#ifndef MODEL_H
#define MODEL_H

// DirectX
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "../ShaderEngine/FxShader.h"
#include "../Misc/Helper.h"

//////////////////////////////////////////////////////////////////////////
struct VertexPNT
{
	VertexPNT():pos(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), tex0(0.0f, 0.0f){}
	VertexPNT(float x, float y, float z, float nx, float ny, float nz, float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv):pos(v), normal(n), tex0(uv){}

	D3DXVECTOR3	pos;
	D3DXVECTOR3	normal;
	D3DXVECTOR2	tex0;
};

//////////////////////////////////////////////////////////////////////////

struct Model
{
public:
	Model();
	~Model();

	bool					LoadEnvironmentMap(const std::string& filename);		// There has to be better place!
	void					Kill();

	// Rendering...
	void					Render();

	// Various Render passes...
	void					RenderColor(LPDIRECT3DDEVICE9 device);
	void					RenderDepthPass(LPDIRECT3DDEVICE9 device);
	void					RenderThroughLight(LPDIRECT3DDEVICE9 device);
	
	
	// Transformation
	void					Translate(const D3DXVECTOR3& axis);
	void					Rotate(const D3DXVECTOR3& axis, float angle);
	void					Scale(const D3DXVECTOR3& axis);
	void					Update(float);
	
	D3DXVECTOR4*			GetWorldPosition();
	unsigned int			GetTriangleCount();
	unsigned int			GetVertexCount();

	// Shading
	void					SetShaderVariables(LPDIRECT3DDEVICE9 device);
	void					SetDepthPassShaderVariables(LPDIRECT3DDEVICE9 device);
	void					SetShaderTechnique(const std::string& tech);
	void					SetMaterialProperties(const D3DXVECTOR4& emm, const D3DXVECTOR4& amb, 
												  const D3DXVECTOR4& diff, const D3DXVECTOR4& spec);

	// Lighting
	void					PointLightIlluminance();
	void					DirectionalLightIlluminance();
	void					SpotLightIlluminance();

	std::vector<LPDIRECT3DVERTEXBUFFER9>	m_vecVB;
	std::vector<LPDIRECT3DINDEXBUFFER9>		m_vecIB;
	std::vector<LPDIRECT3DTEXTURE9>			m_vecDiffTex;
	LPDIRECT3DCUBETEXTURE9					m_pCubeMap;
	
	bool					m_bHasTexcoods;

	int						m_iNumMeshes;

	
	std::vector<int>		m_vecVertexCount;
	std::vector<int>		m_vecIndexCount;
	std::vector<int>		m_vecFaceCount;
	
	VertexPNT*				m_pVertex;
	WORD*					m_pIndex;
	std::string				m_sFilePath;

	/// Shader Support
	std::string				shaderTechnique;
	FxShader*				m_pFxShader;

	// Basic Material Properties
	D3DXVECTOR4				m_colEmissive;
	D3DXVECTOR4				m_colAmbient;
	D3DXVECTOR4				m_colDiffuse;
	D3DXVECTOR4				m_colSpecular;

	/// Transformation
	D3DXMATRIX				matWorld;
	D3DXMATRIX				matTranslate;
	D3DXMATRIX				matRotation;
	D3DXMATRIX				matScale;

};

#endif