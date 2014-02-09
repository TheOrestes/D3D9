
//////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////
float4x4 	matWorld;
float4x4 	matWorldViewProjection;
float4		lightDir;
float4		lightCol;
float4		Eye;

float		Ke = 1.0f;
float		Ka = 0.8f;
float		Kd = 1.0f;
float		Ks = 1.0f;
float		Kr = 0.5f;
float		roughness = 16;

texture 	baseTexture;
texture		envTexture;

sampler2D baseSampler = sampler_state
{
    Texture = <baseTexture>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Anisotropic;
    MagFilter = Anisotropic;
    MipFilter = Anisotropic;
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

//////////////////////////////////////////////////////////////////////
// VERTEX SHADER
//////////////////////////////////////////////////////////////////////
struct VS_INPUT
{
	float4 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION;
	float3 Normal	: TEXCOORD0;
	float2 Tex		: TEXCOORD1;
	float4 Half		: TEXCOORD2;
	float3 R		: TEXCOORD3;
};

VS_OUTPUT mainVS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position = mul(In.Position, matWorldViewProjection);
	In.Normal	 = mul(In.Normal, matWorld);
		
	Out.Normal = normalize(In.Normal);
	Out.Tex 	 = In.Tex;
	
	float4 V = normalize(Eye - In.Position);
	Out.Half = normalize(V + normalize(lightDir));

	//calculate reflection vector
	Out.R = normalize(-reflect(normalize(Eye.xyz), normalize(In.Normal)));
	
	return Out;
}

//////////////////////////////////////////////////////////////////////
// PIXEL SHADER
/////////////////////////////////////////////////////////////////s/////
float4 mainPS(float3 Normal : TEXCOORD0, float3 Tex : TEXCOORD1, float4 Half : TEXCOORD2, float3 R : TEXCOORD3) : COLOR
{
	float4 outColor;
	float facing;

	float4 baseColor = tex2D(baseSampler, Tex);
	
	float NdotL = saturate(dot(Normal, normalize(lightDir.xyz)));
	float NdotH = saturate(dot(Normal, Half)); 
	float4 powNdotH = pow(NdotH, roughness);

	// Ref :- http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
	// had to change the final formula for visual correctness!

	if(NdotL > 0.0f)
		facing = 1.0f;
	else
		facing = 0.0f;

	float4 Emissive = Ke*baseColor;
	float4 Ambient  = Ka*lightCol;
	float4 Diffuse  = Kd*lightCol*NdotL;
	float4 Specular = Ks*lightCol*facing*powNdotH;
	float4 EnvColor = Kr*texCUBE(EnvMapS, R);


	outColor.xyz = Emissive.xyz * (Ambient. xyz + Diffuse.xyz) + Specular.xyz + EnvColor.xyz;
	outColor.w = 1.0f;
	return outColor;
}

//////////////////////////////////////////////////////////////////////
// TECHNIQUES
//////////////////////////////////////////////////////////////////////
technique Specular
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader  = compile ps_3_0 mainPS();
    }
}