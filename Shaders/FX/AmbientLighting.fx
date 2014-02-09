
float4x4	worldViewProjectionMatrix;
float		ambientIntensity;
float4		ambientColor;

//-----------------------------------------------------------------------------
// Vertex Shaders.
//-----------------------------------------------------------------------------

float4 VS_AmbientLighting(float3 position : POSITION) : POSITION
{
    return mul(float4(position, 1.0f), worldViewProjectionMatrix);
}

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_AmbientLighting() : COLOR
{
    return ambientIntensity * ambientColor;
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique AmbientLighting
{
    pass
    {
        VertexShader = compile vs_2_0 VS_AmbientLighting();
        PixelShader = compile ps_2_0 PS_AmbientLighting();
    }
}
