
#include <assert.h>
#include "Model.h"
#include "../Game/Camera.h"
#include "../RenderDevice/RenderDevice.h"
#include "../LightingSystem/LightsManager.h"
#include "../GeometrySystem/GameObject.h"
#include "../LightingSystem/PointLightObject.h"
#include "../LightingSystem/DirectionalLightObject.h"
#include "../Misc/LogManager.h"

//////////////////////////////////////////////////////////////////////////////////////////
Model::Model()
{
	m_vecVB.clear();
	m_vecIB.clear();
	m_vecDiffTex.clear();

	m_iNumMeshes	=	0;
	m_bHasTexcoods	=	true;
	m_vecVertexCount.clear();
	m_vecIndexCount.clear();
	m_vecFaceCount.clear();

	m_sFilePath.clear();

	m_pFxShader		=	new FxShader();

	m_colEmissive	=	D3DXVECTOR4(0.9f, 0.9f, 0.9f, 1.0f);
	m_colAmbient	=   D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f);
	m_colDiffuse	=   D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
	m_colSpecular	=   D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTranslate);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matScale);
};

//////////////////////////////////////////////////////////////////////////////////////////
Model::~Model()
{
	Kill();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Kill()
{
	/// Kill Shader
	LogManager::GetInstance()->WriteToFile("Removing Model ", m_sFilePath.c_str());
	SAFE_DELETE(m_pFxShader);
	
	// Release Textures
	std::vector<LPDIRECT3DTEXTURE9>::iterator iterDiff = m_vecDiffTex.begin();
	for ( ; iterDiff != m_vecDiffTex.end() ; iterDiff++)
	{
		SAFE_RELEASE((*iterDiff));
	}

	// Release Environment Map
	SAFE_RELEASE(m_pCubeMap);

	// Release VBs
	std::vector<LPDIRECT3DVERTEXBUFFER9>::iterator iterVB = m_vecVB.begin();
	for ( ; iterVB != m_vecVB.end() ; iterVB++)
	{
		SAFE_RELEASE((*iterVB));
	}

	// Release IBs
	std::vector<LPDIRECT3DINDEXBUFFER9>::iterator iterIB = m_vecIB.begin();
	for ( ; iterIB != m_vecIB.end() ; iterIB++)
	{
		SAFE_RELEASE((*iterIB));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
bool	Model::LoadEnvironmentMap(const std::string& filename)
{
	LPDIRECT3DDEVICE9 pDevice = RenderDevice::GetInstance()->getDevice();

	HRESULT Hr = D3DXCreateCubeTextureFromFile(pDevice, filename.c_str(), &m_pCubeMap);
	if (S_OK != Hr)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::Render()
{
	LPDIRECT3DDEVICE9 pDevice = RenderDevice::GetInstance()->getDevice();
	
	m_pFxShader->SetShaderTechnique(shaderTechnique);

	if(!shaderTechnique.compare("Diffuse") || !shaderTechnique.compare("Specular") ||
		!shaderTechnique.compare("CookTorrance"))
	{
		/// Set all global shader variables
		SetShaderVariables(pDevice);
		RenderColor(pDevice);

		//RenderThroughLight(pDevice);
		
	}
	else if (!shaderTechnique.compare("DepthPass"))
	{
		SetDepthPassShaderVariables(pDevice);
		RenderDepthPass(pDevice);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::RenderColor(LPDIRECT3DDEVICE9 pDevice)
{
	LPD3DXEFFECT	  pEffect = m_pFxShader->GetEffectPtr();

	UINT uPasses;									
	pEffect->Begin( &uPasses, 0 );

	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{
		pEffect->BeginPass( uPass );

		for ( int i = 0 ; i<m_iNumMeshes ; i++)
		{
			m_pFxShader->SetTextureVariable("envTexture", m_pCubeMap);
			m_pFxShader->SetTextureVariable("baseTexture", m_vecDiffTex[i]);
			pDevice->SetStreamSource( 0, m_vecVB[i], 0, sizeof(VertexPNT) );
			pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );
			pDevice->SetIndices(m_vecIB[i]);
			pEffect->CommitChanges();
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vecIndexCount[i], 0, 
				m_vecFaceCount[i]);
		}

		pEffect->EndPass();
	}

	pEffect->End();
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::RenderDepthPass(LPDIRECT3DDEVICE9 pDevice)
{
	LPD3DXEFFECT	  pEffect = m_pFxShader->GetEffectPtr();

	UINT uPasses;									
	pEffect->Begin( &uPasses, 0 );

	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{
		pEffect->BeginPass( uPass );

		for ( int i = 0 ; i<m_iNumMeshes ; i++)
		{
			pDevice->SetStreamSource( 0, m_vecVB[i], 0, sizeof(VertexPNT) );
			pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );
			pDevice->SetIndices(m_vecIB[i]);
			pEffect->CommitChanges();
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vecIndexCount[i], 0, 
				m_vecFaceCount[i]);
		}

		pEffect->EndPass();
	}

	pEffect->End();
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::RenderThroughLight( LPDIRECT3DDEVICE9 pDevice )
{
	// Create Light ViewMatrix
	D3DXVECTOR3 lightPos(100.0f, 1000.0f, -0.1f);
	D3DXVECTOR3 lightUp(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 lightLook = LightsManager::GetInstance()->GetDirectionalLight(0)->getLightDirection();

	// Get View & Projection Matrices from the Shader Base class
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &lightPos, &lightLook, &lightUp );
	
	D3DXMATRIX matProj;
	D3DXMatrixOrthoLH(&matProj, 20.0f, 20.0f, 1.0f, 50.0f);

	// Create WorldViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = matView;
	D3DXMATRIX matProjVal = matProj;

	D3DXMATRIX wvp;
	D3DXMATRIX wv;

	wv = matWorld * matViewVal;
	wvp = matWorld * matViewVal * matProjVal;

	D3DXVECTOR3 tempEye = lightPos;//Camera::GetInstance()->m_Position;
	D3DXVECTOR4	Eye(tempEye.x, tempEye.y, tempEye.z, 1.0f);

	// Set WorldViewProjection Matrix for Shader

	m_pFxShader->SetMatrixShaderVariable("matWorld", matWorld);
	m_pFxShader->SetMatrixShaderVariable("matWorldView", wv);
	m_pFxShader->SetMatrixShaderVariable("matWorldViewProjection", wvp);
	m_pFxShader->SetVector4ShaderVariable("Eye", Eye);

	// Assign Material Properties.
	m_pFxShader->SetVector4ShaderVariable("EmissiveCol", m_colEmissive);
	m_pFxShader->SetVector4ShaderVariable("SpecularCol", m_colSpecular);
	m_pFxShader->SetVector4ShaderVariable("DiffuseCol", m_colDiffuse);
	m_pFxShader->SetVector4ShaderVariable("AmbientCol", m_colAmbient);

	LPD3DXEFFECT	  pEffect = m_pFxShader->GetEffectPtr();

	UINT uPasses;									
	pEffect->Begin( &uPasses, 0 );

	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{
		pEffect->BeginPass( uPass );

		for ( int i = 0 ; i<m_iNumMeshes ; i++)
		{
			m_pFxShader->SetTextureVariable("envTexture", m_pCubeMap);
			m_pFxShader->SetTextureVariable("baseTexture", m_vecDiffTex[i]);
			pDevice->SetStreamSource( 0, m_vecVB[i], 0, sizeof(VertexPNT) );
			pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 );
			pDevice->SetIndices(m_vecIB[i]);
			pEffect->CommitChanges();
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_vecIndexCount[i], 0, 
				m_vecFaceCount[i]);
		}

		pEffect->EndPass();
	}

	pEffect->End();
}



//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Translate(const D3DXVECTOR3& axis)
{
	D3DXMatrixTranslation(&matTranslate, axis.x, axis.y, axis.z);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Rotate(const D3DXVECTOR3& axis, float angle)
{
	D3DXMatrixRotationAxis(&matRotation, &axis, angle);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Scale(const D3DXVECTOR3& axis)
{
	D3DXMatrixScaling(&matScale, axis.x, axis.y, axis.z);
}

//////////////////////////////////////////////////////////////////////////////////////////
void	Model::Update(float dt)
{
	matWorld = matScale * matRotation * matTranslate;	
}

//////////////////////////////////////////////////////////////////////////////////////////
D3DXVECTOR4* Model::GetWorldPosition()
{
	D3DXVECTOR4 pos(matWorld._41, matWorld._42, matWorld._43, matWorld._44);
	return &pos;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int Model::GetTriangleCount()
{
	return m_vecFaceCount.size();
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int Model::GetVertexCount()
{
	return m_vecVertexCount.size();
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::SetShaderVariables(LPDIRECT3DDEVICE9 device)
{
	// Get View & Projection Matrices from the Shader Base class
	D3DXMATRIX* matView = RenderDevice::GetInstance()->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = RenderDevice::GetInstance()->GetGlobalProjectionMatrix();

	// Create WorldViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	D3DXMATRIX wvp;
	D3DXMATRIX wv;

	wv = matWorld * matViewVal;
	wvp = matWorld * matViewVal * matProjVal;

	D3DXVECTOR3 tempEye = Camera::GetInstance()->m_Position;
	D3DXVECTOR4	Eye(tempEye.x, tempEye.y, tempEye.z, 1.0f);

	// Set WorldViewProjection Matrix for Shader
	
	m_pFxShader->SetMatrixShaderVariable("matWorld", matWorld);
	m_pFxShader->SetMatrixShaderVariable("matWorldView", wv);
	m_pFxShader->SetMatrixShaderVariable("matWorldViewProjection", wvp);
	m_pFxShader->SetVector4ShaderVariable("Eye", Eye);

	// Lighting
	DirectionalLightIlluminance();
	PointLightIlluminance();

	// Assign Material Properties.
	m_pFxShader->SetVector4ShaderVariable("EmissiveCol", m_colEmissive);
	m_pFxShader->SetVector4ShaderVariable("SpecularCol", m_colSpecular);
	m_pFxShader->SetVector4ShaderVariable("DiffuseCol", m_colDiffuse);
	m_pFxShader->SetVector4ShaderVariable("AmbientCol", m_colAmbient);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::SetDepthPassShaderVariables(LPDIRECT3DDEVICE9 device)
{
	// Get View & Projection Matrices from the Shader Base class
	D3DXMATRIX* matView = RenderDevice::GetInstance()->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = RenderDevice::GetInstance()->GetGlobalProjectionMatrix();

	// Create WorldViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	D3DXMATRIX wvp;
	D3DXMATRIX wv;

	wv = matWorld * matViewVal;
	wvp = matWorld * matViewVal * matProjVal;

	D3DXVECTOR3 tempEye = Camera::GetInstance()->m_Position;
	D3DXVECTOR4	Eye(tempEye.x, tempEye.y, tempEye.z, 1.0f);

	// Set WorldViewProjection Matrix for Shader
	m_pFxShader->SetMatrixShaderVariable("matWorldView", wv);
	m_pFxShader->SetMatrixShaderVariable("matWorldViewProjection", wvp);
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::PointLightIlluminance()
{
	// Lights related variables...
	int nPointLights = LightsManager::GetInstance()->GetPointLightCount();
	m_pFxShader->SetFloatShaderVariable("numPtLights", nPointLights);

	LPD3DXEFFECT pEffect = m_pFxShader->GetEffectPtr();
	
	// Point Light related shader handles...
	D3DXHANDLE hPtLight;
	D3DXHANDLE hPtLightPos;
	D3DXHANDLE hPtLightColor;
	D3DXHANDLE hPtLightIntensity;
	D3DXHANDLE hPtLightVelocity;
	D3DXHANDLE hPtLightRadius;

	for (int i = 0 ; i < nPointLights ; ++i)
	{
		PointLightObject* currPointLight = LightsManager::GetInstance()->GetPointLight(i);

		D3DXVECTOR3 LightPosition	= currPointLight->getLightPosition();
		D3DXVECTOR4	LightColor		= currPointLight->getLightColor();
		float		LightRadius		= currPointLight->getLightRadius();
		float		LightIntensity	= currPointLight->getLightIntensity();
		D3DXVECTOR3	LightVelocity	= currPointLight->getLightVelocity();	

		hPtLight = pEffect->GetParameterElement("ptLights", i);

		hPtLightPos = pEffect->GetParameterByName(hPtLight, "pos");
		hPtLightColor = pEffect->GetParameterByName(hPtLight, "color");
		hPtLightRadius = pEffect->GetParameterByName(hPtLight, "radius");
		hPtLightIntensity = pEffect->GetParameterByName(hPtLight, "intensity");
		hPtLightVelocity = pEffect->GetParameterByName(hPtLight, "velocity");

		pEffect->SetValue(hPtLightPos, LightPosition, sizeof(LightPosition));
		pEffect->SetValue(hPtLightColor, LightColor, sizeof(LightColor));
		pEffect->SetFloat(hPtLightRadius, LightRadius);
		pEffect->SetFloat(hPtLightIntensity, LightIntensity);
		pEffect->SetValue(hPtLightVelocity, LightVelocity, sizeof(LightVelocity));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::DirectionalLightIlluminance()
{
	// Lights related variables...
	int nDirectionalLights = LightsManager::GetInstance()->GetDirectionalLightCount();
	m_pFxShader->SetFloatShaderVariable("numDirLights", nDirectionalLights);

	LPD3DXEFFECT pEffect = m_pFxShader->GetEffectPtr();

	// Directional Light related shader handles...
	D3DXHANDLE hDirLight;
	D3DXHANDLE hDirLightDir;
	D3DXHANDLE hDirLightColor;
	D3DXHANDLE hDirLightIntensity;
	
	for (int i = 0 ; i < nDirectionalLights ; ++i)
	{
		DirectionalLightObject* currDirLight = LightsManager::GetInstance()->GetDirectionalLight(i);

		D3DXVECTOR3 LightDir		= currDirLight->getLightDirection();
		D3DXVECTOR4	LightColor		= currDirLight->getLightColor();
		float		LightIntensity	= currDirLight->getLightIntensity();

		hDirLight = pEffect->GetParameterElement("dirLights", i);

		hDirLightDir = pEffect->GetParameterByName(hDirLight, "dir");
		hDirLightColor = pEffect->GetParameterByName(hDirLight, "color");
		hDirLightIntensity = pEffect->GetParameterByName(hDirLight, "intensity");

		pEffect->SetValue(hDirLightDir, LightDir, sizeof(LightDir));
		pEffect->SetValue(hDirLightColor, LightColor, sizeof(LightColor));
		pEffect->SetFloat(hDirLightIntensity, LightIntensity);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::SetShaderTechnique( const std::string& tech )
{
	shaderTechnique = tech;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Model::SetMaterialProperties( const D3DXVECTOR4& emm, const D3DXVECTOR4& amb, 
									const D3DXVECTOR4& diff, const D3DXVECTOR4& spec )
{
	m_colEmissive = emm;
	m_colAmbient  = amb;
	m_colDiffuse  = diff;
	m_colSpecular = spec;
}








