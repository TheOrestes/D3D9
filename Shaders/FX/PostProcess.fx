// Global Shader Variables
texture gScreenTexture;
texture gNoiseTexture;

texture gBaseTexture;

// Matrices
extern float4x4 worldMatrix;
extern float4x4 worldviewMatrix;
extern float4x4 viewMatrix;
extern float4x4 wvpMatrix;
extern float4 eye;

// Timer
float delta;
extern float3 LightDir;

// Rain Drops Effect
texture gRainTexture;
float	xSpeed = 0.0f;
float	ySpeed = 0.02f;

// Toon Shader
texture gToonTexture;

// Image Processing KERNEL
static const float Kernel[9] = 
{
	0, 1, 0, 1, -4, 1, 0, 1, 0		// Sobel Edge Kernel
};

// Step Size, ( 1/textureWidth, 1/textureHeight)
float step = 1.0f/512.0f;

// number of samples
float Sum = 1.0f;

// Samplers
sampler screenSampler = sampler_state
{
    Texture = <gScreenTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler baseSampler = sampler_state
{
    Texture = <gBaseTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler noiseSampler = sampler_state
{
    Texture = <gNoiseTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler rainSampler = sampler_state
{
    Texture = <gRainTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler toonSampler = sampler_state
{
    Texture = <gToonTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


struct VS_OUTPUT
{
    float4 Pos:POSITION;
    float2 Tex:TEXCOORD0;
};

//////////////////////////// Toon Rendering ////////////////////////////
struct VS_INPUT_BASE
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
};

struct VS_OUTPUT_BASE
{
	float4 Pos		: POSITION;
	float2 Tex		: TEXCOORD0;
	float4 vLight	: TEXCOORD1;
	float3 vNormal  : TEXCOORD2;
};

VS_OUTPUT_BASE vs_main(VS_INPUT_BASE In)
{
	VS_OUTPUT_BASE output = (VS_OUTPUT_BASE)0;

	output.Pos = mul(In.Pos, wvpMatrix);
	
	output.vLight  = normalize(mul(float4(LightDir, 0.0f), viewMatrix));
	output.vNormal = normalize(mul(float4(In.Normal, 0.0f), worldMatrix));

	output.Tex = In.Tex;

	return output;
}

float4 ps_main(VS_OUTPUT_BASE In) : COLOR0
{
	float u  = dot(In.vLight, In.vNormal);
	
	// get in a range [0,1]
	u = (u*0.5f) + 0.5f;
	float v = 0.5f;

	float2 toonUV;
	toonUV.x = u;
	toonUV.y = v;

	// Use this texture lookup color value if you need base texture color
	//tex2D(baseSampler, In.Tex);

	return ((2.0f*float4(0.5f, 0.8f, 0.0f, 1.0f)) * tex1D(toonSampler, toonUV.x));
}
////////////////////////////////////////////////////////////////////////

// Convolution
float4 Convolve3x3(VS_OUTPUT IN)
{
	float2 ox = float2(step, 0.0);
	float2 oy = float2(0.0, step);

	float2 P   = IN.Tex.xy - oy;
	float4 C00 = tex2D( screenSampler, P-ox);
	float4 C01 = tex2D( screenSampler, P);
	float4 C02 = tex2D( screenSampler, P+ox);

	P = IN.Tex.xy;
	float4 C10 = tex2D( screenSampler, P-ox);
	float4 C11 = tex2D( screenSampler, P);
	float4 C12 = tex2D( screenSampler, P+ox);

	P = IN.Tex.xy + oy;
	float4 C20 = tex2D( screenSampler, P-ox);
	float4 C21 = tex2D( screenSampler, P);
	float4 C22 = tex2D( screenSampler, P+ox);

	float4 Ci = C00 * Kernel[0];
	Ci += C01 * Kernel[1];
	Ci += C02 * Kernel[2];
	Ci += C10 * Kernel[3];
	Ci += C11 * Kernel[4];
	Ci += C12 * Kernel[5];
	Ci += C20 * Kernel[6];
	Ci += C21 * Kernel[7];
	Ci += C22 * Kernel[8];

	return (Ci/Sum);
}

/////////////////////////////////////////////////////////////////////

// The vertex shader does not do any transformation, coordinates
// are allready in clipping space.
VS_OUTPUT VS( float4 Pos:POSITION,  float2 Tex:TEXCOORD0 )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos=Pos;
    output.Tex = Tex;
    return output;
}

//////////////////////////////////////////////////////////////////////////////////////////
// SSAO
//////////////////////////////////////////////////////////////////////////////////////////
float4 PS_SSAO(VS_OUTPUT pIn) : COLOR0
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}


// Default
float4 PS(VS_OUTPUT pIn):COLOR0
{
    float4 rgb = tex2D ( screenSampler, pIn.Tex);
    return rgb; 
}

// Monochrome
float4 PS_MONOCHROME(VS_OUTPUT pIn):COLOR0
{
	float4 rgb = tex2D ( screenSampler, pIn.Tex);
    float4 mean = ( rgb.r + rgb.g + rgb.b ) / 3.0f;
    return mean; //float4( mean, mean, mean, 1.0f );
}

// Pixelate
float4 PS_PIXEL(VS_OUTPUT pIn):COLOR0
{
	pIn.Tex.x = round( pIn.Tex.x * 100) / 100;
	pIn.Tex.y = round( pIn.Tex.y * 100) / 100;
    float4 rgb = tex2D ( screenSampler, pIn.Tex);
    return rgb;
}

// Distorted Glass
float4 PS_GLASS(VS_OUTPUT pIn):COLOR0
{
	pIn.Tex.x += sin(pIn.Tex.x*750)*0.01;
	pIn.Tex.y += sin(pIn.Tex.x*750)*0.003;
    float4 rgb = tex2D ( screenSampler, pIn.Tex)*1.5;
    return rgb; 
}

// Old Tv
float4 PS_OLDTV(VS_OUTPUT pIn):COLOR0
{
    pIn.Tex.y += sin(delta*1000)*0.01;
    pIn.Tex.x += cos(delta*3000)*0.01;

    float4 rgb = tex2D ( screenSampler, pIn.Tex);

    rgb.r = rgb.r*sin(pIn.Tex.y*1000000*delta)*3;

    float mean = ( rgb.r + rgb.g + rgb.b ) / 3.0f;
    return float4(mean, mean, mean, 1.0);
}

// Black n White
float4 PS_BW(VS_OUTPUT pIn):COLOR0
{
	float4 rgb = tex2D ( screenSampler, pIn.Tex);
    float4 mean = ( rgb.r + rgb.g + rgb.b ) / 3.0f;
    
    if (mean.r < 0.5)
		mean.r = 0.0f;
	else
		mean.r = 1.0f;

	mean.gb = mean.r;
	
	return mean;
}

// NightVision
float4 PS_NIGHT(VS_OUTPUT pIn):COLOR0
{
    float4 rgb = tex2D ( screenSampler, pIn.Tex );

    float4 visionCol = float4(0.1, 0.95, 0.2, 1.0); 
    return  rgb * visionCol;// * noiseColor; 
}

// Laplace
float4 PS_LAPLACE(VS_OUTPUT pIn):COLOR0
{
	float4 rgb = Convolve3x3(pIn);
    return rgb; 
}

float4 PS_RAIN(VS_OUTPUT pIn):COLOR0
{
	float2 temp;
	temp.x = pIn.Tex.x + xSpeed*delta;
	temp.y = pIn.Tex.y + ySpeed*delta;

	float2 pixel = tex2D(rainSampler, temp).rg * 5;
	temp.x = pIn.Tex.x + pixel.x * 0.01f;
	temp.y = pIn.Tex.y + pixel.y * 0.01f;
    return float4(tex2D(screenSampler, temp*(1-pixel.x*0.01f)).rgb, 1.0); 
}

float4 PS_TOON(VS_OUTPUT pIn):COLOR0
{
    float4 rgb = tex2D ( screenSampler, pIn.Tex );
    rgb.r = round(rgb.r*3) / 3;
	rgb.g = round(rgb.g*3) / 3;
	rgb.b = round(rgb.b*3) / 3;
    return  rgb;
}


// Testing
float4 PS_TEST(VS_OUTPUT pIn):COLOR0
{
	//float4 rgb = tex2D ( screenSampler, pIn.Tex ); 
	float4 rgb = Convolve3x3(pIn);
    return rgb; 
}

/////////////////////////////////////////////////////////////////////
// tecnique name needs to correspond to the value passed
// to the GetTechniqueByName method
technique Default
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS();
    }
}

technique Monochrome
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_MONOCHROME();
    }
}

technique NightVision
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_NIGHT();
    }
}

technique BlacknWhite
{
	pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_BW();
    }
}

technique Pixelate
{
	pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_PIXEL();
    }
}

technique BlurGlass
{
	pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_GLASS();
    }
}

technique OldTV
{
	pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_OLDTV();
    }
}

technique Laplace
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_LAPLACE();
    }
}

technique RainDrops
{
    pass p0
    {
        VertexShader = compile vs_3_0 VS();
        PixelShader = compile ps_3_0 PS_RAIN();
    }
}

technique Toon
{
    pass p0
    {
        VertexShader = compile vs_3_0 vs_main();
        PixelShader = compile ps_3_0 ps_main();
    }
}

