// ***************************************************************
//  FxShader    
//  -------------------------------------------------------------
//  Purpose :	Individual FX Shader
// ***************************************************************


#ifndef FXSHADER_H
#define FXSHADER_H

#include <d3dx9.h>
#include <assert.h>
#include <vector>
#include <map>
#include "Shader.h"

class FxShader : public Shader
{
public:
	FxShader();
	virtual		~FxShader();

	virtual void		Kill();

	virtual void		CreateShader(const std::string& name);
	void				InitShaderTechniques();
	void				SetShaderTechnique(const std::string& tech);
	LPD3DXEFFECT		GetEffectPtr();

	virtual void		SetFloatShaderVariable(const std::string& name, float val);
	virtual void		SetValue(const std::string& name, const void* val);
	virtual void		SetValueByParameter(const std::string& name, const std::string& paramName, 
											int index, const void* val);
	virtual void		SetVector3ShaderVariable(const std::string& name, const D3DXVECTOR3& val);
	virtual void		SetVector4ShaderVariable(const std::string& name, const D3DXVECTOR4& val);
	virtual void		SetMatrixShaderVariable(const std::string& name, const D3DXMATRIX& val);
	virtual	void		SetTextureVariable(const std::string& name, LPDIRECT3DTEXTURE9	tex);
	virtual	void		SetTextureVariable(const std::string& name, LPDIRECT3DCUBETEXTURE9 tex);

private:

	LPD3DXEFFECT		m_pEffect;
	std::string			m_pEffectFileName;

	std::map<std::string, int>	mapShaderType;

	void				CacheTextures();
	void				ReleaseTextures();
	


};

#endif 