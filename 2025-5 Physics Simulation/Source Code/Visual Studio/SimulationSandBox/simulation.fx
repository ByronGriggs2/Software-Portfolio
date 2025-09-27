//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix normalMatrix;
    float4 lightPos;
    float4 cameraPos;
    float4 baseColor;
    int texType;
    float padding[3];
}
//--------------------------------------------------------------------------------------
float4 calculateLighting(float4 baseColor, float3 pos, float3 normal, float4 lightPos);
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 lightPos : TEXCOORD0;
    float4 cameraPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float3 worldPos : TEXCOORD3;
    float4 baseColor : TEXCOORD4;
};
VS_OUTPUT VS(float4 Pos : POSITION, float3 Norm : NORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, World);
    output.worldPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.normal = mul(Norm, (float3x3) normalMatrix).xyz;
    output.normal = normalize(output.normal);
    output.lightPos = lightPos;
    output.cameraPos = cameraPos;
    output.baseColor = baseColor;
    return output;
}
float4 PS(VS_OUTPUT input) : SV_Target
{
    //return float4(input.normal.x, input.normal.y, input.normal.z, 1);
    return calculateLighting(input.baseColor, input.worldPos, input.normal, input.lightPos);
}
struct VS_OUTPUT_TEXTURE
{
    float4 Pos : SV_POSITION;
    float4 lightPos : TEXCOORD0;
    float4 cameraPos : TEXCOORD1;
    float3 normal : TEXCOORD2;
    float3 worldPos : TEXCOORD3;
    float2 tex : TEXCOORD4;
    int texType : TEXCOORD5;
};

VS_OUTPUT_TEXTURE VS_TEXTURE(float4 Pos : POSITION, float3 Norm : NORMAL, float2 Tex : TEXCOORD)
{
    VS_OUTPUT_TEXTURE output = (VS_OUTPUT_TEXTURE) 0;
    output.Pos = mul(Pos, World);
    output.worldPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.normal = mul(Norm, (float3x3) normalMatrix).xyz;
    output.normal = normalize(output.normal);
    output.lightPos = mul(lightPos, World);
    output.cameraPos = cameraPos;
    output.tex = Tex;
    output.texType = texType;
    return output;
}

SamplerState defaultSampler : register(s0);
Texture2D defaultTexture : register(t0);

float4 PS_TEXTURE(VS_OUTPUT_TEXTURE input) : SV_Target
{
    float4 myTexture = float4(0, 0, 0, 0);
    if (input.texType == 0)
        myTexture = defaultTexture.Sample(defaultSampler, input.tex);
    
    return calculateLighting(myTexture, input.worldPos, input.normal, input.lightPos);
}

float4 calculateLighting(float4 baseColor, float3 pos, float3 normal, float4 lightPos)
{
    float4 materialDiff = baseColor;
    float4 materialSpec = float4(1.0, 1.0, 1.0, 1.0);
    float4 lightCol = float4(1.0, 1.0, 1.0, 1.0);
    
    float3 lightDir = normalize(lightPos.xyz - pos);
    float diff = max(0.0, dot(lightDir, normal));
    
    return float4((0.5 * baseColor + diff * materialDiff) * lightCol);
}