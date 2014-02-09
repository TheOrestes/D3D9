// ***************************************************************
//  SceneLight			date: 17/2/2010
//  -------------------------------------------------------------
//  Purpose :	To add support for the standard D3D Lighting.
//  -------------------------------------------------------------
//  DoD -> Copyright (C) 2010 - All Rights Reserved
// ***************************************************************

#include <d3d9.h>
#include <d3dx9.h>

class SceneLight
{
public:

	enum LightType
	{
		OMNI = 0,
		DIRECTIONAL = 1,
		SPOT = 2
	};

	SceneLight();
	~SceneLight();

	void		initOmniLight();
	void		initDirectionalLight();
	void		initSpotLight();

	D3DLIGHT9	initLight(LightType type);
	void		DestroyLight();
	void		SwitchOnLight(bool flag);

	D3DMATERIAL9 initMtrl();

	D3DXVECTOR3 getLightPosition(){ return Position; }
	D3DXVECTOR3 getLightDirection(){ return Direction; }

	LightType	getLightType(){ return SceneLightType; }
	void		SetLightType(LightType type){ SceneLightType = type; }


private:

	LightType	SceneLightType;

	D3DLIGHT9	OmniLight;
	D3DLIGHT9	DirLight;
	D3DLIGHT9	SpotLight;

	// Light Properties...
	D3DXCOLOR	AmbientColor;
	D3DXCOLOR	DiffuseColor;
	D3DXCOLOR	SpecularColor;
	float		Intensity;
	D3DXVECTOR3	Position;
	D3DXVECTOR3 Direction;
	float		Range;
	float		Spot_Falloff;
	float		Spot_Phi;
	float		Spot_Theta;

	// Material Properties...
	D3DXCOLOR	MatAmbient;
	D3DXCOLOR	MatDiffuse;
	D3DXCOLOR	MatSpecular;
	D3DXCOLOR	MatEmissive;
	float		Power;
};




