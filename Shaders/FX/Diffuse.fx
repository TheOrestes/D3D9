
//////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////
#define MAX_POINT_LIGHTS 3
#define MAX_DIR_LIGHTS 1

float4x4 	matWorld;
float4x4 	matWorldView;
float4x4 	matWorldViewProjection;
float4		Eye;

float		roughness = 0.8;

// Object Material Properties
float4		EmissiveCol;
float4		AmbientCol;
float4		DiffuseCol;
float4      SpecularCol;		// Just to avoid Invalid Handle D3DX warning.

struct PointLight
{
	float3	pos;
	float4	color;
	float	radius;
	float	intensity;
	float3  velocity;
};

float		numPtLights;
PointLight  ptLights[MAX_POINT_LIGHTS];

//--- Directional Light
struct DirLight
{
	float3 dir;
	float4 color;
	float  intensity;
};

float		numDirLights;
DirLight	dirLights[MAX_DIR_LIGHTS];

// Textures
texture 	baseTexture;
texture		envTexture;		// might be needed in future!

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
// Oren Nayar Model
//////////////////////////////////////////////////////////////////////
float4 OrenNayarSimple(float3 normal, float3 light, float3 eye)
{
    // Make sure the interpolated inputs and
    // constant parameters are normalized
    float3 n = normalize( normal );
    float3 l = normalize( light );
    float3 v = normalize( eye );
 
    // Compute the other aliases
    float gamma   = dot( v - n * dot( v, n ), l - n * dot( l, n ) );
 
    float rough_sq = roughness * roughness;
 
    float A = 1.0f - 0.5f * (rough_sq / (rough_sq + 0.57f));
 
    float B = 0.45f * (rough_sq / (rough_sq + 0.09));
 
    float C;
   
    float alpha = max( acos( dot( v, n ) ), acos( dot( l, n ) ) );
    float beta  = min( acos( dot( v, n ) ), acos( dot( l, n ) ) );
 
	C = sin(alpha) * tan(beta);
 
	float3 final = (A + B * max( 0.0f, gamma ) * C);
 
    return float4( final, 1.0f );
}

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
	float4 WorldPos	: TEXCOORD2;
	float3 Eye		: TEXCOORD3;
};

VS_OUTPUT mainVS(VS_INPUT In)
{
	VS_OUTPUT Out;
	
	Out.Position =	mul(In.Position, matWorldViewProjection);
	In.Normal	 =	mul(In.Normal, matWorld);	
	Out.Normal	 =	In.Normal;
	Out.Tex 	 =	In.Tex;
	Out.Eye      =	(Eye.xyz - In.Position.xyz);
	Out.WorldPos =	mul(In.Position, matWorld);

	
	return Out;
}

//////////////////////////////////////////////////////////////////////
// PIXEL SHADER
/////////////////////////////////////////////////////////////////s/////
float4 mainPS(VS_OUTPUT IN) : COLOR
{
	float4 outColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 N = normalize(IN.Normal);
	float3 I = normalize(IN.Eye);
	float3 L = float3(0.0f, 0.0f, 0.0f);
	float3 H = float3(0.0f, 0.0f, 0.0f);

	float atten = 0.0f;
	float NdotL = 0.0f;

	float4 DiffusePoint  = float4(0.0f, 0.0f, 0.0f, 0.0f); 
	float4 SpecularPoint = float4(0.0f, 0.0f, 0.0f, 0.0f); 

	float4 DiffuseDir  = float4(0.0f, 0.0f, 0.0f, 0.0f); 
	float4 SpecularDir = float4(0.0f, 0.0f, 0.0f, 0.0f); 

	// Extract Texture Color
	float4 baseColor = tex2D(baseSampler, IN.Tex);

	for(int i = 0 ; i<numPtLights ; ++i)
	{
		L = (ptLights[i].pos - IN.WorldPos) / ptLights[i].radius;
		atten = saturate(1.0f - dot(L,L));
	
		L = normalize(L);
		H = normalize(L + I);

		NdotL = saturate(dot(N,L)); 
		//NdotL = OrenNayarSimple(N, L, I);
		
		DiffusePoint += ptLights[i].color * atten * NdotL * ptLights[i].intensity;
	}

	// Consider Directional Light's illumination
	for(int i = 0 ; i<numDirLights ; ++i)
	{
		// reverse the direction, because we want N & L to be in same direction
		// for correct dot product
		// refer how for point lights direction is calculated!
		// vec(0,1,0) means, light is pointing towards Sky, but for a lighting calcualtion,
		// it should be reversed. 
		L = -normalize(dirLights[i].dir);

		NdotL = saturate(dot(N,L)); 
		//NdotL = OrenNayarSimple(N, L, I);
		
		DiffuseDir += dirLights[i].color * NdotL * dirLights[i].intensity;
	}
		

	// Ref :- http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
	// had to change the final formula for visual correctness!
	
	float4 finalDiffuse  = (DiffusePoint + DiffuseDir) * DiffuseCol;
	
	float4 Emissive = EmissiveCol * baseColor;
	float4 Ambient  = AmbientCol;

	outColor.xyz = Emissive.xyz * (Ambient. xyz + finalDiffuse.xyz);
	outColor.w = 1.0f;
	return outColor;
}

//////////////////////////////////////////////////////////////////////
// TECHNIQUES
//////////////////////////////////////////////////////////////////////
technique Diffuse
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader  = compile ps_3_0 mainPS();
    }
}
