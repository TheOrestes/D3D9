#include "FxShader.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
FxShader::FxShader()
{
	m_pEffect			=	NULL;

	mapShaderType.clear();
	InitShaderTechniques();
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
FxShader::~FxShader()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Initialize MAP for Shader Techniques
//------------------------------------------------------------------------
void	FxShader::InitShaderTechniques()
{
	mapShaderType["Default"] = 0;
	mapShaderType["Diffuse"] = 1;
	mapShaderType["Specular"] = 2;
	mapShaderType["NormalMap"] = 3;
	mapShaderType["EnvMap"] = 4;
	mapShaderType["CookTorrance"] = 5;
	mapShaderType["DepthPass"] = 6;
	mapShaderType["SSAO_PrePassPosition"] = 7;
	mapShaderType["SSAO_PrePassNormal"] = 8;
}

//------------------------------------------------------------------------
// Purpose  : Create Fx Shader from file
//------------------------------------------------------------------------
void	FxShader::CreateShader(const std::string& type)
{
	HRESULT hr;
	
	IDirect3DDevice9*	gDevice = RenderDevice::GetInstance()->getDevice();
	int shaderType = mapShaderType[type];

	switch (shaderType)
	{
	case 0:
		m_pEffectFileName = "Shaders\\FX\\Default.fx";
		break;

	case 1:
		m_pEffectFileName = "Shaders\\FX\\Diffuse.fx";
		break;

	case 2:
		m_pEffectFileName = "Shaders\\FX\\Specular.fx";
		break;

	case 3:
		m_pEffectFileName = "Shaders\\FX\\NormalMap.fx";
		break;

	case 4:
		m_pEffectFileName = "Shaders\\FX\\EnvMap.fx";
		break;

	case 5:
		m_pEffectFileName = "Shaders\\FX\\CookTorrance.fx";
		break;

	case 6:
		m_pEffectFileName = "Shaders\\FX\\DepthPass.fx";
		break;

	case 7:
		m_pEffectFileName = "Shaders\\FX\\SSAO_PrePass.fx";
		break;

	case 8:
		m_pEffectFileName = "Shaders\\FX\\SSAO_PrePass.fx";
		break;

	default:
		m_pEffectFileName = "Shaders\\FX\\Default.fx";
		break;
	}

	LPD3DXBUFFER	errors = NULL;
	hr = D3DXCreateEffectFromFile(	gDevice, 
									m_pEffectFileName.c_str(),
									NULL,
									NULL,
									0,
									NULL,
									&m_pEffect,
									&errors);

	if (hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile((char*)errors->GetBufferPointer());
		assert("FxShader::CreateShader");
	}
	else
	{
		LogManager::GetInstance()->WriteToFile(m_pEffectFileName.c_str(), "Effect Created...");
	}
}

//------------------------------------------------------------------------ 
// Purpose  : Get Effect Pointer
//------------------------------------------------------------------------
LPD3DXEFFECT	FxShader::GetEffectPtr()
{
	return m_pEffect;
}

//------------------------------------------------------------------------
// Purpose  : Set float shader variable
//-----------------------------------------------------------------------
void	FxShader::SetFloatShaderVariable(const std::string& name, float val)
{
	D3DXHANDLE	h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetFloat(h, val);
}

//------------------------------------------------------------------------
// Purpose  : Set Vector4 type shader variable
//------------------------------------------------------------------------
void	FxShader::SetVector3ShaderVariable(const std::string& name, const D3DXVECTOR3& val)
{
	D3DXHANDLE	h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());
	
	// Set value for that variable handle
	m_pEffect->SetValue(h, val, sizeof(val));
}

//------------------------------------------------------------------------
// Purpose  : Set Any type of shader variable
//------------------------------------------------------------------------
void	FxShader::SetValue(const std::string& name, const void* val)
{
	D3DXHANDLE	h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetValue(h, val, sizeof(val));
}

//------------------------------------------------------------------------
// Purpose  : Set Any type of shader variable by parameter
//------------------------------------------------------------------------
void	FxShader::SetValueByParameter(const std::string& name, const std::string& paramName, int index, const void* val)
{
	D3DXHANDLE	h;
	D3DXHANDLE  elemHandle;

	elemHandle = m_pEffect->GetParameterElement(paramName.c_str(), index);

	// Get variable handle
	h = m_pEffect->GetParameterByName(elemHandle, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetValue(h, val, sizeof(val));
}

//------------------------------------------------------------------------
// Purpose  : Set Vector3 type shader variable
//------------------------------------------------------------------------
void	FxShader::SetVector4ShaderVariable(const std::string& name, const D3DXVECTOR4& val)
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
void	FxShader::SetMatrixShaderVariable(const std::string& name, const D3DXMATRIX& val)
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
void	FxShader::SetTextureVariable(const std::string& name, LPDIRECT3DTEXTURE9 tex)
{
	D3DXHANDLE h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetTexture(h, tex);
}

//------------------------------------------------------------------------
// Purpose  : Set Cubemap texture
//------------------------------------------------------------------------
void	FxShader::SetTextureVariable(const std::string& name, LPDIRECT3DCUBETEXTURE9 tex)
{
	D3DXHANDLE h;

	// Get variable handle
	h = m_pEffect->GetParameterByName(0, name.c_str());

	// Set value for that variable handle
	m_pEffect->SetTexture(h, tex);
}

//------------------------------------------------------------------------
// Purpose  : Start Shader Function
//------------------------------------------------------------------------
void	FxShader::SetShaderTechnique(const std::string& type)
{
	int shaderType = mapShaderType[type];

	switch (shaderType)
	{
	case 0:
		m_pEffect->SetTechnique("Default");
		break;

	case 1:
		m_pEffect->SetTechnique("Diffuse");
		break;

	case 2:
		m_pEffect->SetTechnique("Specular");
		break;

	case 3:
		m_pEffect->SetTechnique("NormalMap");
		break;

	case 4:
		m_pEffect->SetTechnique("EnvMap");
		break;

	case 5:
		m_pEffect->SetTechnique("CookTorrance");
		break;

	case 6:
		m_pEffect->SetTechnique("DepthPass");
		break;

	case 7:
		m_pEffect->SetTechnique("SSAO_PrePassPosition");
		break;

	case 8:
		m_pEffect->SetTechnique("SSAO_PrePassNormal");
		break;
	default:
		m_pEffect->SetTechnique("Default");
		break;
	}
}

//------------------------------------------------------------------------ 
// Purpose  : kill THIS FxShader
//-----------------------------------------------------------------------
void	FxShader::Kill()
{
	SAFE_RELEASE(m_pEffect);
	LogManager::GetInstance()->WriteToFile("FxShader Shutdown...");
}

