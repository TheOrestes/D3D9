
#include "../RenderDevice/RenderDevice.h"
#include "../LightingSystem/SceneLight.h"
#include "../Misc/LogManager.h"

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Default Constructor
//------------------------------------------------------------------------
SceneLight::SceneLight()
{
	// Light Type...
	SceneLightType	= DIRECTIONAL;

	// Light Properties...
	AmbientColor	= D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		
	DiffuseColor	= D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);		
	SpecularColor	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		
	Intensity		= 1000.0f;
	Position		= D3DXVECTOR3(1.0f, 5.0f, 0.0f);
	Direction		= D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	Range			= 1000.0f;
	Spot_Falloff	= 0.2f;
	Spot_Phi		= D3DX_PI/6.0f;									// Outer cone angle
	Spot_Theta		= D3DX_PI/18.0f;								// Inner cone angle

	// Material Properties...
	MatAmbient		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	MatDiffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	MatEmissive		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	MatSpecular		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Power			= 32.0f;
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Destructor
//------------------------------------------------------------------------
SceneLight::~SceneLight()
{
	DestroyLight();
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Initialize Light
//------------------------------------------------------------------------
D3DLIGHT9 SceneLight::initLight(LightType type)
{
	D3DLIGHT9 temp;

	switch (type)
	{
	case 0:
		{	
			initDirectionalLight();
			temp = DirLight;
			LogManager::GetInstance()->WriteToFile("Directional Light Created...");
		} 
		break;

	case 1:
		{
			initOmniLight();
			temp = OmniLight;
			LogManager::GetInstance()->WriteToFile("Omni Light Created...");
		} 
		break;

	case 2:
		{
			initSpotLight();
			temp = SpotLight;
			LogManager::GetInstance()->WriteToFile("Spot Light Created...");
		}
		break;

	default:
		break;
	}

	return temp;
}
//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Directional Light
//------------------------------------------------------------------------
void SceneLight::initDirectionalLight()
{
	::ZeroMemory(&DirLight, sizeof(DirLight));

	DirLight.Type		= D3DLIGHT_DIRECTIONAL;
	DirLight.Ambient	= AmbientColor;
	DirLight.Diffuse	= DiffuseColor;
	DirLight.Specular	= SpecularColor * 0.6f;
	DirLight.Direction	= Direction;
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Omni Light
//------------------------------------------------------------------------
void SceneLight::initOmniLight()
{
	::ZeroMemory(&OmniLight, sizeof(OmniLight));

	OmniLight.Type		= D3DLIGHT_POINT;
	OmniLight.Ambient	= AmbientColor;
	OmniLight.Diffuse	= DiffuseColor;
	OmniLight.Specular	= SpecularColor * 0.6f;
	OmniLight.Position	= Position;
	OmniLight.Attenuation2 = 0.1f;
	OmniLight.Range		= Range;
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Spot Light
//------------------------------------------------------------------------
void SceneLight::initSpotLight()
{
	::ZeroMemory(&SpotLight, sizeof(SpotLight));

	SpotLight.Type		= D3DLIGHT_SPOT;
	SpotLight.Position  = Position;
	SpotLight.Direction = Direction;
	SpotLight.Ambient	= AmbientColor;
	SpotLight.Diffuse	= DiffuseColor;
	SpotLight.Specular	= SpecularColor * 0.6f;
	SpotLight.Falloff	= Spot_Falloff;
	SpotLight.Attenuation1 = 0.1f;
	SpotLight.Range		= Range;
	SpotLight.Theta		= Spot_Theta;
	SpotLight.Phi		= Spot_Phi;
	
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Initialize Material
//------------------------------------------------------------------------
D3DMATERIAL9 SceneLight::initMtrl()
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient	=	MatAmbient;
	mtrl.Diffuse	=	MatDiffuse;
	mtrl.Specular	=	MatSpecular;
	mtrl.Emissive	=	MatEmissive;
	mtrl.Power		=	Power;

	return mtrl;
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Turn On/Off Lights
//------------------------------------------------------------------------
void	SceneLight::SwitchOnLight(bool flag)
{
	D3DLIGHT9 temp = initLight(LightType(0));

	RenderDevice::GetInstance()->getDevice()->SetLight(0, &temp);
	RenderDevice::GetInstance()->getDevice()->LightEnable(0, flag);

	LogManager::GetInstance()->WriteToFile("Lights On...");
}

//------------------------------------------------------------------------
// Date     : 17/2/2010
// Purpose  : Destroy created light
//------------------------------------------------------------------------
void	SceneLight::DestroyLight()
{
	LogManager::GetInstance()->WriteToFile("Lights removed...");
}


