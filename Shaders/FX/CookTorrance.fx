
//////////////////////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////////////////////
#define MAX_POINT_LIGHTS 3
#define MAX_DIR_LIGHTS 1

float4x4 	matWorld;
float4x4	matWorldView;
float4x4 	matWorldViewProjection;
float4		lightDir;
float4		lightCol;
float4		Eye;

// Object Material Properties
float4		EmissiveCol;
float4		AmbientCol;
float4		DiffuseCol;
float4		SpecularCol;
float		Kr = 0.4f;
float		roughness = 0.5;

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
// Cook Torrance BRDF
// http://content.gpwiki.org/index.php/D3DBook:%28Lighting%29_Cook-Torrance
//////////////////////////////////////////////////////////////////////
float4 CookTorranceBRDF(float3 normal, float3 Eye, float3 lightDir, float3 Half)
{
    float NdotL        = saturate( dot( normal, lightDir ) );
    float NdotH        = saturate( dot( normal, Half ) );
    float NdotV        = saturate( dot( normal, Eye ) );
    float VdotH        = saturate( dot( Eye, Half ) );
    float r_sq         = roughness * roughness;

	// Evaluate the geometric term
    // --------------------------------
    float geo_numerator   = 2.0f * NdotH;
    float geo_denominator = VdotH;
 
    float geo_b = (geo_numerator * NdotV ) / geo_denominator;
    float geo_c = (geo_numerator * NdotL ) / geo_denominator;
    float geo   = min( 1.0f, min( geo_b, geo_c ) );

	// Now evaluate the roughness term
    // -------------------------------
	float roughness_a = 1.0f / ( 4.0f * r_sq * pow( NdotH, 4 ) );
    float roughness_b = NdotH * NdotH - 1.0f;
    float roughness_c = r_sq * NdotH * NdotH;
 
    float roughness_value = roughness_a * exp( roughness_b / roughness_c );

	// Next evaluate the Fresnel value
	// http://filmicgames.com/archives/557
    // -------------------------------
    float base = 1 - VdotH;
	float exponential = pow( base, 5.0);
	float F0 = 0.05f;
	float fresnel = exponential + F0 * (1.0 - exponential);
	
	// Put all the terms together to compute
    // the specular term in the equation
    // -------------------------------------
    float3 Rs_numerator   = ( fresnel * geo * roughness_value );
    float Rs_denominator  = NdotV * NdotL;
    float3 Rs             = Rs_numerator / Rs_denominator;

	float4 finalSpec = float4(Rs, 1.0f);

	return finalSpec;
}

float4 PhongBRDF( float3 normal, float3 Eye, float3 reflection )
{    
    // Compute the angle between the reflection and the viewer
    float  RdotV        = saturate( dot( reflection, Eye ));
 
    // Compute the specular colour
    float3 specular     = pow( RdotV, 16.0f );

    // Determine the final colour    
    return float4( specular, 1.0f );
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
	Out.Eye      =	Eye.xyz - In.Position.xyz;
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
	float Spec = 0.0f;

	float4 DiffusePoint  = float4(0.0f, 0.0f, 0.0f, 0.0f); 
	float4 SpecularPoint = float4(0.0f, 0.0f, 0.0f, 0.0f); 

	float4 DiffuseDir  = float4(0.0f, 0.0f, 0.0f, 0.0f); 
	float4 SpecularDir = float4(0.0f, 0.0f, 0.0f, 0.0f); 

	//calculate reflection vector
	float3 R = -normalize(reflect(I, N));
	float4 baseColor = tex2D(baseSampler, IN.Tex);

	for(int i = 0 ; i<numPtLights ; ++i)
	{
		L = (ptLights[i].pos - IN.WorldPos) / ptLights[i].radius;
		atten = saturate(1.0f - dot(L,L));
	
		L = normalize(L);
		H = normalize(L + I);

		NdotL = saturate(dot(N,L));
		//Spec =	PhongBRDF(N,I,R);//
		Spec =	CookTorranceBRDF(N, I, L, H);
		//torranceSpec =	PhongBRDF(N, I, R);
		
		DiffusePoint += ptLights[i].color * atten * NdotL * ptLights[i].intensity;
		SpecularPoint += Spec * atten * ptLights[i].color * ptLights[i].intensity;
	}

	// Consider Directional Light's illumination
	for(int i = 0 ; i<numDirLights ; ++i)
	{
		L = -normalize(dirLights[i].dir);

		H = normalize(L+I);

		NdotL = saturate(dot(N,L));
		Spec =	PhongBRDF(N, I, R);

		DiffuseDir += dirLights[i].color * NdotL * dirLights[i].intensity;
		SpecularDir += Spec * dirLights[i].color * dirLights[i].intensity;
	}
	
	// Ref :- http://http.developer.nvidia.com/CgTutorial/cg_tutorial_chapter05.html
	// had to change the final formula for visual correctness!
	
	float4 finalDiffuse  = (DiffusePoint + DiffuseDir) * DiffuseCol;
	float4 finalSpecular = (SpecularPoint + SpecularDir) * SpecularCol;
	
	float4 Emissive = EmissiveCol * baseColor;
	float4 Ambient  = AmbientCol;
	float4 EnvColor = Kr * texCUBE(EnvMapS, R);

	outColor.xyz = Emissive.xyz * (Ambient. xyz + finalDiffuse.xyz) + finalSpecular.xyz + EnvColor.xyz;
	outColor.w = 1.0f;
	return outColor;
}

//////////////////////////////////////////////////////////////////////
// TECHNIQUES
//////////////////////////////////////////////////////////////////////
technique CookTorrance
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader  = compile ps_3_0 mainPS();
    }
}