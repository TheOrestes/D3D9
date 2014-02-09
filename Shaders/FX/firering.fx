//=============================================================================
// firering.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Oscillates particles in a fiery motion.
//=============================================================================

uniform extern float4x4 matWVP;
uniform extern float4x4 matWorld;
uniform extern float4x4 matView;
uniform extern float4x4 matProj;
uniform extern texture  gTex;
uniform extern float3   gEyePosL;
uniform extern float3   gAccel;
uniform extern float    gTime;
uniform extern float3   gCamPos;
uniform extern int      gViewportHeight;

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

OutputVS FireRingVS(float3 posL    : POSITION0, 
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
	
	// Rotate the particles about local space.
	float sine, cosine;
	sincos(0.5f*mass*t, sine, cosine);
	float x = posL.x*cosine + posL.y*-sine;
	float y = posL.x*sine + posL.y*cosine;
	
	// Oscillate particles up and down.
	float s = sin(6.0f*t);
	posL.x = x;
	posL.y = y + mass*s;
	
	// Constant acceleration.
	posL = posL + vel*t;// + 0.5f * gAccel * t * t;
	
	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(posL, 1.0f), matWVP);
		
	// Ramp up size over time.
	size += 2.0f*t*t;
	
	// Also compute size as a function of the distance from the camera,
	// and the viewport heights.  The constants were found by 
	// experimenting.
	float d = distance(posL, gEyePosL);
	outVS.size = size; //gViewportHeight*size/(1.0f + 8.0f*d);
	
	// Fade color from white to black over time to fade particles out.
	outVS.color = (1.0f - (t / lifeTime));
	
	// Done--return the output.
    return outVS;
}

float4 FireRingPS(float4 color : COLOR0, float2 tex0 : TEXCOORD0) : COLOR
{
	return color*tex2D(TexS, tex0);
}

technique FireRingTech
{
    pass P0
    {
        vertexShader = compile vs_3_0 FireRingVS();
        pixelShader  = compile ps_3_0 FireRingPS();
        
        PointSpriteEnable = true;
        AlphaBlendEnable = true;
	    SrcBlend     = One;
	    DestBlend    = One;
	    	    
	    // Don't write to depth buffer; that is, particles don't obscure
	    // the objects behind them.
	    ZWriteEnable = false;
    }
}
