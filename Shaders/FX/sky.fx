//=============================================================================
// sky.fx by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================

uniform extern float4x4 matWVP;
uniform extern texture  gEnvMap;

float delta;
float fTime;

sampler EnvMapS = sampler_state
{
    Texture   = <gEnvMap>;
    MinFilter = LINEAR; 
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};


void SkyVS(float3 posL : POSITION0, 
           out float4 oPosH : POSITION0, 
           out float3 oEnvTex : TEXCOORD0)
{
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    oPosH = mul(float4(posL, 1.0f), matWVP).xyww; 
    
    // Use skymesh vertex position, in local space, as index into cubemap. 
    oEnvTex = posL;
}

float4 SkyPS(float3 envTex : TEXCOORD0) : COLOR
{
    return texCUBE(EnvMapS, envTex);
}

technique SkyTech
{
    pass P0
    {
        vertexShader = compile vs_2_0 SkyVS();
        pixelShader  = compile ps_2_0 SkyPS();
		CullMode = None;
		//ZFunc = Always; // Always write sky to depth buffer
		/*StencilEnable = true;
		StencilFunc   = Always;
		StencilPass   = Replace;
		StencilRef    = 0; // clear to zero*/
    }
}

