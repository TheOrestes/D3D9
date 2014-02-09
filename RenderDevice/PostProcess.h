
// ***************************************************************
//  PostProcess			
//  -------------------------------------------------------------
//  Purpose :	Post Processing Framework
// ***************************************************************

#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include <d3dx9.h>
#include "../Misc/Helper.h"
#include "../GeometrySystem/Vertex.h"

class PostProcess
{
public:
	PostProcess();
	virtual ~PostProcess();

	void	Kill();
	bool	PostProcessInit(LPDIRECT3DDEVICE9 device);
	void	RenderToTextureBegin(LPDIRECT3DDEVICE9 device);
	void	RenderToTextureEnd(LPDIRECT3DDEVICE9 device);
	void	RenderProcessedQuad(LPDIRECT3DDEVICE9 device);
	void	RenderProcessedQuadWithSSAO(LPDIRECT3DDEVICE9 device);

	void	SetTechnique(const std::string& tech);
	void	SetTexture(const std::string& texname, LPDIRECT3DTEXTURE9 device);
	void	SetFloatShaderVariable(const std::string& varname, float val);
	void	SetMatrixShaderVariable(const std::string& matName, const D3DXMATRIX& val);
	void	SetTextureVariable(const std::string& texname, LPDIRECT3DTEXTURE9 device);
	void	SetVector4ShaderVariable(const std::string& name , const D3DXVECTOR4& val);
private:
	LPDIRECT3DVERTEXBUFFER9		m_pFullScreenVB;
	LPD3DXEFFECT				m_pEffect;

	LPDIRECT3DTEXTURE9			m_pRTTexture;
	LPDIRECT3DSURFACE9			m_pRTSurface;
	LPDIRECT3DSURFACE9			m_pBBSurface;
};

#endif




