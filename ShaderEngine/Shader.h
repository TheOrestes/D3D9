// ***************************************************************
//  IShader     
//  -------------------------------------------------------------
//  Purpose :	Base Class for Shaders
// ***************************************************************

#ifndef SHADER_H
#define SHADER_H

#include <d3dx9.h>
#include <assert.h>

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void		Kill();
};

#endif