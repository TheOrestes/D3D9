
//////////////////////////////////////////////////////////////////////////////////
// Globals
uniform extern float4x4 matWVP;
uniform extern float4x4 matWorld;
uniform extern float4x4 matView;
uniform extern float4x4 matProj;
uniform extern texture  gTex;
uniform extern float3   gEyePosL;
uniform extern float3   gAccel;
uniform extern float    gTime;
uniform extern float3   gCamPos;

sampler TexS = sampler_state
{
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU  = CLAMP;
    AddressV  = CLAMP;
};
 
struct OutputVS
{
    float4 posH  : POSITION0;
	float4 color : COLOR0;
    float2 tex0  : TEXCOORD0; // D3D fills in for point sprites.
    float size   : PSIZE; // In pixels.
};

OutputVS CampVS(float3 posL    : POSITION0, 
                float3 vel     : TEXCOORD0,
                float size     : TEXCOORD1,
                float time     : TEXCOORD2,
                float lifeTime : TEXCOORD3,
                float mass     : TEXCOORD4,
                float4 color   : COLOR0)
{
    // Zero out our output.
	OutputVS outVS = (OutputVS)0;
	
	// Get age of particle from creation time.
	float t = gTime - time;
	
	// Constant acceleration.
	posL = posL + vel*t + 0.5f * gAccel * t * t;
	float3 posW = mul(float4(posL, 1.0f), matWorld);
	
	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), matWVP);
		
	// distance based size
	float d = distance(posW, gCamPos);
	outVS.size = 15.0f * size * (1/d); 

	outVS.color = (1.0f - (t / lifeTime));
	
	// Done--return the output.
    return outVS;
}

float4 CampPS(float4 color : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{

	return color*tex2D(TexS, tex0);
}

technique CampFireTech
{
    pass P0
    {
        vertexShader = compile vs_3_0 CampVS();
        pixelShader  = compile ps_3_0 CampPS();
        
        PointSpriteEnable = true;
        AlphaBlendEnable = true;
	    SrcBlend     = One;
	    DestBlend    = One;
	    	    
	    // Don't write to depth buffer; that is, particles don't obscure
	    // the objects behind them.
	    ZWriteEnable = false;
    }
}

