
#include "PostProcess.h"
#include "RenderDevice.h"
#include "../Misc/LogManager.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
PostProcess::PostProcess()
{
	m_pBBSurface	=	NULL;
	m_pEffect		=	NULL;
	m_pFullScreenVB	=	NULL;
	m_pRTSurface	=	NULL;
	m_pRTTexture	=	NULL;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
PostProcess::~PostProcess()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : initialize PostProcessing Params
//------------------------------------------------------------------------
bool	PostProcess::PostProcessInit(LPDIRECT3DDEVICE9 pDevice)
{
	// Load Post Process Effect
	LPD3DXBUFFER	pErrBuffer = NULL;
	HRESULT HrEffect = D3DXCreateEffectFromFile(pDevice, 
												"Shaders\\FX\\PostProcess.fx", 
												NULL, NULL, 
												D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER, 
												NULL, 
												&m_pEffect, 
												&pErrBuffer);

	if (FAILED(HrEffect))
	{
		LogManager::GetInstance()->WriteToFile((char*)pErrBuffer->GetBufferPointer());
		return false;
	}

	// get info from current frame buffer
	pDevice->GetRenderTarget(0, &m_pBBSurface);

	// get the description for the texture
	D3DSURFACE_DESC	desc;
	m_pBBSurface->GetDesc(&desc);

	// Create texture from back-buffer description
	pDevice->CreateTexture(	desc.Width, 
							desc.Height, 
							1, D3DUSAGE_RENDERTARGET, 
							D3DFMT_A16B16G16R16F, 
							D3DPOOL_DEFAULT, 
							&m_pRTTexture, 
							NULL);

	// Get surface level 0 of created texture
	m_pRTTexture->GetSurfaceLevel(0, &m_pRTSurface);

	// Create VB for Screen Aligned Quad
	HRESULT Hr = pDevice->CreateVertexBuffer(4*sizeof(VertexPP), 
											D3DUSAGE_WRITEONLY, 
											D3DFVF_XYZ|D3DFVF_TEX1, 
											D3DPOOL_DEFAULT, 
											&m_pFullScreenVB, 
											NULL);

	if(FAILED(Hr))
	{
		MessageBox(NULL, NULL, TEXT("CreateVertexBuffer FAILED"), MB_ICONERROR);
		return false;
	}

	// Push data into our screen aligned Quad
	VertexPP*	m_Quad = NULL;
	m_pFullScreenVB->Lock(0, 0, (void**)&m_Quad, 0);
	m_Quad[0] = VertexPP(-1.0f, 1.0f, 0.0f, 1.0f, 0.0f,0.0f);
	m_Quad[1] = VertexPP(1.0f, 1.0f, 0.0f, 1.0f, 1.0f,0.0f);
	m_Quad[2] = VertexPP(-1.0f,-1.0f, 0.0f, 1.0f, 0.0f,1.0f);
	m_Quad[3] = VertexPP(1.0f,-1.0f, 0.0f, 1.0f, 1.0f,1.0f);
	m_pFullScreenVB->Unlock();

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Begin Render To Texture
//------------------------------------------------------------------------
void	PostProcess::RenderToTextureBegin(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderTarget(0, m_pRTSurface);
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xffeeeeee, 1.0f, 0);

	pDevice->BeginScene();
}

//------------------------------------------------------------------------
// Purpose  : End Render to texture
//------------------------------------------------------------------------
void	PostProcess::RenderToTextureEnd(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderTarget(0, m_pBBSurface);
	pDevice->EndScene();
}

//------------------------------------------------------------------------
// Purpose  : 
//------------------------------------------------------------------------
void PostProcess::RenderProcessedQuad( LPDIRECT3DDEVICE9 pDevice)
{
	// Post Processing Technique
	m_pEffect->SetTechnique("Default");
	pDevice->SetVertexDeclaration(VertexPP::pVertDecl);

	// Post Processing vars
	m_pEffect->SetTexture("gScreenTexture", m_pRTTexture);

	UINT uPasses;
	m_pEffect->Begin(&uPasses, 0);

	for (UINT uPass = 0; uPass < uPasses; ++uPass)
	{
		m_pEffect->BeginPass(uPass);

		pDevice->SetStreamSource(0, m_pFullScreenVB, 0, sizeof(VertexPP));
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pEffect->EndPass();
	}

	m_pEffect->End();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Render with SSAO 
//////////////////////////////////////////////////////////////////////////////////////////
void PostProcess::RenderProcessedQuadWithSSAO( LPDIRECT3DDEVICE9 pDevice)
{
	// Post Processing Technique
	m_pEffect->SetTechnique("SSAO");
	pDevice->SetVertexDeclaration(VertexPP::pVertDecl);

	// Post Processing vars
	m_pEffect->SetTexture("gScreenTexture", m_pRTTexture);

	UINT uPasses;
	m_pEffect->Begin(&uPasses, 0);

	for (UINT uPass = 0; uPass < uPasses; ++uPass)
	{
		m_pEffect->BeginPass(uPass);

		pDevice->SetStreamSource(0, m_pFullScreenVB, 0, sizeof(VertexPP));
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		m_pEffect->EndPass();
	}

	m_pEffect->End();
}

//------------------------------------------------------------------------
// Purpose  : 
//------------------------------------------------------------------------
void	PostProcess::SetTechnique(const std::string& tech)
{
	m_pEffect->SetTechnique(tech.c_str());
}

//------------------------------------------------------------------------
// Purpose  : Set float shader variable
//-----------------------------------------------------------------------
void	PostProcess::SetFloatShaderVariable(const std::string& name, float val)
{
	D3DXHANDLE	h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetFloat(h, val);
}

//------------------------------------------------------------------------
// Purpose  : Set Vector type shader variable
//------------------------------------------------------------------------
void	PostProcess::SetVector4ShaderVariable(const std::string& name, const D3DXVECTOR4& val)
{
	D3DXHANDLE	h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetVector(h, &val);
}

//------------------------------------------------------------------------
// Purpose  : Set Matrix type shader variable
//------------------------------------------------------------------------
void	PostProcess::SetMatrixShaderVariable(const std::string& name, const D3DXMATRIX& val)
{
	D3DXHANDLE h;

	// Get variable handle name
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetMatrix(h, &val);
}

//------------------------------------------------------------------------
// Purpose  : Set Texture
//------------------------------------------------------------------------
void	PostProcess::SetTextureVariable(const std::string& name, LPDIRECT3DTEXTURE9 tex)
{
	D3DXHANDLE h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetTexture(h, tex);
}

//------------------------------------------------------------------------
// Purpose  : Kill
//------------------------------------------------------------------------
void	PostProcess::Kill()
{
	SAFE_RELEASE(m_pBBSurface);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pFullScreenVB);
	SAFE_RELEASE(m_pRTSurface);
	SAFE_RELEASE(m_pRTTexture);
}



