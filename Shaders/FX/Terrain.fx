//=============================================================================
// Terrain.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Blends three textures together with a blend map.
//=============================================================================

float4x4 gMatWorld;
float4x4 gMatWVP;
float4  gLightDir;
texture gTex0;
texture gTex1;
texture gTex2;
texture gBlendMap;

static float gTexScale = 32.0f;

sampler Tex0S = sampler_state
{
	Texture = <gTex0>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

sampler Tex1S = sampler_state
{
	Texture = <gTex1>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

sampler Tex2S = sampler_state
{
	Texture = <gTex2>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

sampler BlendMapS = sampler_state
{
	Texture = <gBlendMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	AddressU  = WRAP;
	AddressV  = WRAP;
};

struct VS_INPUT
{
	float3 Position :	POSITION0;
	float3 Normal	:	NORMAL0;
	float2 Tex		:	TEXCOORD0;
};
 
struct VS_OUTPUT
{
    float4 posH         : POSITION0;
    float2 tiledTexC    : TEXCOORD0;
    float2 nonTiledTexC : TEXCOORD1;
    float  shade        : TEXCOORD2;
};

VS_OUTPUT TerrainVS(VS_INPUT In)
{
    // Zero out our output.
	VS_OUTPUT outVS = (VS_OUTPUT)0;
	
	// Just compute a grayscale diffuse and ambient lighting 
	// term--terrain has no specular reflectance.  The color 
	// comes from the texture.
	//In.Normal = normalize(mul(In.Normal, gMatWorld));
	
	outVS.shade = saturate(dot(In.Normal, normalize(gLightDir.xyz))) + 0.4f;
    
	// Transform to homogeneous clip space.
	outVS.posH = mul(float4(In.Position, 1.0f), gMatWVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
	outVS.tiledTexC    = In.Tex * gTexScale; // Scale tex-coord to tile.
	outVS.nonTiledTexC = In.Tex; // Blend map not tiled.
	
	// Done--return the output.
    return outVS;
}

float4 TerrainPS(float2 tiledTexC : TEXCOORD0, 
                 float2 nonTiledTexC : TEXCOORD1,
                 float shade : TEXCOORD2) : COLOR0
{
	// Layer maps are tiled
    float3 c0 = tex2D(Tex0S, tiledTexC).rgb;
    float3 c1 = tex2D(Tex1S, tiledTexC).rgb;
    float3 c2 = tex2D(Tex2S, tiledTexC).rgb;
    
    // Blendmap is not tiled.
    float3 B = tex2D(BlendMapS, nonTiledTexC).rgb;

	// Find the inverse of all the blend weights so that we can
	// scale the total color to the range [0, 1].
    float totalInverse = 1.0f / (B.r + B.g + B.b);
    
    // Scale the colors by each layer by its corresponding weight
    // stored in the blendmap.  
    c0 *= B.r * totalInverse;
    c1 *= B.g * totalInverse;
    c2 *= B.b * totalInverse;
    
    // Sum the colors and modulate with the shade to brighten/darken.
    float3 final = (c0 + c1 + c2) * shade ;
    
    return float4(final, 1.0f);
}

technique Terrain
{
    pass P0
    {
        vertexShader = compile vs_3_0 TerrainVS();
        pixelShader  = compile ps_3_0 TerrainPS();

		//FillMode = Wireframe;
		//CullMode = None;
    }
}
