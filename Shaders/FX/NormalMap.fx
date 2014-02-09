//////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////
float4x4 	matWorld;
float4x4 	matWorldViewProjection;
float4		lightDir;
float4		Eye;

float		Kd = 0.6f;
float		Ks = 1.0f;
float		Ke = 0.3f;
float		roughness = 16;

float4		IncandColor = float4(0.05f, 0.05f, 0.05f, 1.0f);

texture 	baseTexture;
texture		normalmapTexture;
texture		envTexture;

sampler normalSampler = sampler_state
{
    Texture   = <normalmapTexture>;
    MinFilter = LINEAR; 
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler EnvMapS = sampler_state
{
    Texture   = <envTexture>;
    MinFilter = LINEAR; 
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler2D baseSampler = sampler_state
{
    Texture = <baseTexture>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Anisotropic;
    MagFilter = Anisotropic;
    MipFilter = Anisotropic;
};

//////////////////////////////////////////////////////////////////////
// VERTEX SHADER
//////////////////////////////////////////////////////////////////////
struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
	float3 BiNormal	: BINORMAL;
	float3 Tangent	: TANGENT;
	float2 Tex		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 LightDir : TEXCOORD0;
	float2 Tex		: TEXCOORD1;
	float3 Half		: TEXCOORD2;
	float3 R		: TEXCOORD3;	
};

VS_OUTPUT mainVS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position = mul(In.Position, matWorldViewProjection);
	In.Normal	 = mul(In.Normal, matWorld);
	
	// Pass-on Default texcoord as it is!	
	Out.Tex 	 = In.Tex;

	float3 TangentW = normalize(mul(In.Tangent, matWorld));
	float3 BiNormalW = normalize(mul(In.BiNormal, matWorld));
	float3 NormalW = normalize(mul(In.Normal, matWorld)); 

	// Tangent Space Matrix
	float3x3 TangentSpace;
	TangentSpace[0] = TangentW;
	TangentSpace[1] = BiNormalW;
	TangentSpace[2] = NormalW;

	// Tranform light direction vector into Tangent Space
	Out.LightDir = mul(lightDir.xyz, TangentSpace);

	//calculate reflection vector
	Out.R = normalize(-reflect(normalize(Eye.xyz), normalize(In.Normal)));
	
	float3 Half = normalize(normalize(Eye.xyz - In.Position.xyz) + normalize(lightDir.xyz));
	Out.Half = mul(Half, TangentSpace);
	
	return Out;
}

//////////////////////////////////////////////////////////////////////
// PIXEL SHADER
//////////////////////////////////////////////////////////////////////
float4 mainPS(float3 LightDir : TEXCOORD0, float3 Tex : TEXCOORD1, float3 Half : TEXCOORD2, float3 R : TEXCOORD3) : COLOR
{
	float4 baseColor = tex2D(baseSampler, Tex) * 1.75f;
	
	// Extract Normals from Normal Map
	float3 Normal = normalize(tex2D(normalSampler, Tex) * 2 - 1);

	// Base Texture look-up
	float4 Diffuse = max(0,dot(Normal, normalize(LightDir)));
	
	float4 Specular = pow(dot(Normal, normalize(Half)), roughness);

	float4 EnvColor = texCUBE(EnvMapS, R);
	
	return (Kd*baseColor*Diffuse); // + Ks*Specular/*+ Ke*EnvColor*/);
}

//////////////////////////////////////////////////////////////////////
// TECHNIQUES
//////////////////////////////////////////////////////////////////////
technique NormalMap
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader  = compile ps_3_0 mainPS();
    }
}