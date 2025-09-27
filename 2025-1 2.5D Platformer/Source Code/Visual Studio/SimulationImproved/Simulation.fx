//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    float4 lightPos;
    float4 cameraPos;
    float time;
    float spotlightCB;
    int texType;
    float buffer;
}
//calculates diffusion, ambient and spectral lighting
float4 calculateLighting(float spotlight, float3 normal, float3 sunPos, float4 finalPos, float3 posWorld, float4 myCamera, bool texturedObject);
//random number generator
float hash(float n)
{
    return frac(sin(n) * 43758.5453123) * 2.0 - 1.0;
}
//explicit truncation used to convert World from 4x4 to 3x4
float3x4 trunc(float4x4 mat)
{
    return float3x4(
        mat._11, mat._12, mat._13, mat._14,
        mat._21, mat._22, mat._23, mat._24,
        mat._31, mat._32, mat._33, mat._34
    );
}
//--------------------------------------------------------------------------------------
//default vertex shader has the parameters for every other vertex shader combined. This allows its input layout to be used for every vertex shader
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
    float2 Tex : TEXCOORD1;
    int texType : TEXCOORD2;
    float3 PosWorld : TEXCOORD3;
    float4 lightPos : TEXCOORD4;
    float4 cameraPos : TEXCOORD5;
    float spotlight : TEXCOORD6;
};
//used for textures as well
VS_OUTPUT VS(float4 Pos : POSITION, float3 Norm : NORMAL, float2 Tex : TEXCOORD, float3 tangent : TANGENT, float3 binormal : BINORMAL)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Norm = Norm;
    output.Tex = Tex;
    output.texType = texType;
    output.PosWorld = mul(Pos, World).xyz;
    output.lightPos = lightPos;
    output.cameraPos = cameraPos;
    output.spotlight = spotlightCB;
    return output;
}
//This sampler is used for everything except the skybox
SamplerState basicSampler : register(s0);
//The defualt PS is only used for the arrow (look direction indicator)
float4 PS(VS_OUTPUT input) : SV_Target
{
    return float4(1, 1, 0, 1);
}
//--------------------------------------------------------------------------------------
//Gourad shading
struct VS_OUTPUT_GOURAD
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};
VS_OUTPUT_GOURAD VS_gourad(float4 Pos : POSITION, float3 N : NORMAL)
{
    VS_OUTPUT_GOURAD output = (VS_OUTPUT_GOURAD) 0;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    float3 posWorld = mul(Pos, World).xyz;
    float3 normal = mul(normalize(N), trunc(World)).xyz;
    output.Color = calculateLighting(spotlightCB, normal, lightPos.xyz, output.Pos, posWorld, cameraPos, false);
    return output;
    
}
float4 PS_gourad(VS_OUTPUT_GOURAD input) : SV_Target
{
    return input.Color;
}
//--------------------------------------------------------------------------------------
//phong shading
struct VS_OUTPUT_PHONG
{
    float4 Pos : SV_POSITION;
    float3 Norm : TEXCOORD0;
    float3 PosWorld : TEXCOORD1;
    float4 lightPos : TEXCOORD2;
    float4 cameraPos : TEXCOORD3;
    float spotlight : TEXCOORD4;
};
VS_OUTPUT_PHONG VS_phong(float4 Pos : POSITION, float3 N : NORMAL)
{
    VS_OUTPUT_PHONG output = (VS_OUTPUT_PHONG) 0;
    output.Pos = mul(Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    output.PosWorld = mul(Pos, World).xyz;
    output.Norm = mul(normalize(N), trunc(World)).xyz;
    output.lightPos = lightPos;
    output.cameraPos = cameraPos;
    output.spotlight = spotlightCB;
    return output;
}
float4 PS_phong(VS_OUTPUT_PHONG input) : SV_Target
{
    float4 finalColor = calculateLighting(input.spotlight, input.Norm, input.lightPos.xyz, float4(input.PosWorld, 1), input.PosWorld, input.cameraPos, false);
    return finalColor;
}
//--------------------------------------------------------------------------------------
//Skybox/environment
struct VS_OUTPUT_SKY
{
    float4 Pos : SV_POSITION;
    float3 viewDir : TEXCOORD0;
};
VS_OUTPUT_SKY VS_sky(float4 Pos : POSITION)
{
    VS_OUTPUT_SKY output = (VS_OUTPUT_SKY) 0;
    float4 inPos = mul(Pos, World);
    output.viewDir = normalize(inPos.xyz - cameraPos.xyz);
    inPos = mul(inPos, View);
    inPos = mul(inPos, Projection);
    output.Pos = inPos;
    return output;
}
TextureCube txSkyColor : register(t0);
SamplerState txSkySampler : register(s1);
float4 PS_sky(VS_OUTPUT_SKY input) : SV_Target
{
    return txSkyColor.Sample(txSkySampler, input.viewDir);
}
//--------------------------------------------------------------------------------------
//Bump AND displacement mapping
struct VS_OUTPUT_BUMP
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD1;
    float3 lightDirInTangent : TEXCOORD2;
    float3 originalN : TEXCOORD3;
    float4 cameraPos : TEXCOORD4;
    float4 worldPos : TEXCOORD5;
    float spotlight : TEXCOORD6;
};
VS_OUTPUT_BUMP VS_bump(float4 Pos : POSITION, float3 N : NORMAL, float2 Tex : TEXCOORD, float3 tangent : TANGENT, float3 binormal : BINORMAL)
{
    VS_OUTPUT_BUMP output = (VS_OUTPUT_BUMP) 0;
    output.worldPos = mul(Pos, World);
    output.Pos = mul(output.worldPos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = Tex;
    output.originalN = N;
    
    float3 viewDirW = cameraPos.xyz - output.worldPos.xyz;
    float3 lightDirW;
    if (-0.1 <= spotlightCB && 0.1 >= spotlightCB)
    {
        lightDirW = lightPos.xyz - output.worldPos.xyz;
    }
    else
    {
        lightDirW = cameraPos.xyz - output.worldPos.xyz;
    }

    // Transform normal, tangent, and binormal to world space and normalize
    float3 newN = normalize(mul(N, (float3x3) World));
    float3 T = normalize(mul(tangent, (float3x3) World));
    float3 B = normalize(mul(binormal, (float3x3) World));

    // Construct the tangent space matrix
    float3x3 mat2Tang = float3x3(T, B, newN);

    // Transform light direction to tangent space
    output.lightDirInTangent = normalize(mul(mat2Tang, lightDirW));
    output.originalN = mul(mat2Tang, newN);
    output.spotlight = spotlightCB;
    output.cameraPos = cameraPos;
    return output;
}
Texture2D rockNormal : register(t5);
Texture2D rocks : register(t6);
float4 PS_bump(VS_OUTPUT_BUMP input) : SV_Target
{
    float4 rocksTexture = rocks.Sample(basicSampler, input.Tex);
    float4 rocksNormal = rockNormal.Sample(basicSampler, input.Tex);
    
    //create bumps
    const float RADIUS = 0.06;
    const float2 GRIDSIZE = float2(0.2, 0.2);
    const float2 CENTEROFFSET = float2(0.1, 0.1);
    float2 cellCenter = floor(input.Tex / GRIDSIZE) * GRIDSIZE + CENTEROFFSET;
    float2 dist = input.Tex - cellCenter;
    
    float3 N;
    if (dot(dist, dist) <= RADIUS * RADIUS)
    {
        float z = sqrt(RADIUS * RADIUS - dot(dist, dist));
        N = normalize(float3(dist.x, dist.y, z));

    }
    else
    {
        N = input.originalN;
    }
    //N is the bump mapping component, rocksNormal is the displacement mapping component
    N = normalize(N + 0.25 * rocksNormal.xyz);
    float diff = max(dot(N, input.lightDirInTangent), 0.0f);
    
    float4 finalColor;
    //spotlight
    if (-0.1 <= input.spotlight && 0.1 >= input.spotlight)
    {
        finalColor = rocksTexture * (diff + 0.2);
    }
    else
    {
        float spotLightSize = 20.0f * (distance(input.cameraPos.xyz, input.worldPos.xyz) / 80.0f);
        if (sqrt(pow(input.cameraPos.x - input.worldPos.x, 2.0) + pow(input.cameraPos.y - input.worldPos.y-12, 2.0)) <= spotLightSize)
        {
            finalColor = rocksTexture * (diff + 0.2);
        }
        else
        {
            finalColor = rocksTexture * (diff+0.2)*0.2;
        }
    }

    return finalColor;
}
//--------------------------------------------------------------------------------------
//toon shading
struct VS_OUTPUT_TOON
{
    float4 Pos : SV_POSITION;
    float4 originalPos : TEXCOORD1;
    float4 light : TEXCOORD2;
};
VS_OUTPUT_TOON VS_toon(float4 Pos : POSITION, float3 N : NORMAL)
{
    VS_OUTPUT_TOON output = (VS_OUTPUT_TOON) 0;
    output.originalPos = Pos;
    float4 posWorld = mul(Pos, World);
    output.Pos = mul(posWorld, View);
    output.Pos = mul(output.Pos, Projection);
    float3 normal = normalize(mul(N, trunc(World))).xyz;
    output.light = calculateLighting(spotlightCB, normal, lightPos.xyz, output.Pos, posWorld.xyz, cameraPos, false);
    return output;
}
float4 PS_toon(VS_OUTPUT_TOON input) : SV_Target
{
    //fmod(<variable>/6.0f,5.0f)/5.0f creates a gradient. Flooring (<variable>/6.0f) seperates it into distinct shades
    float4 color1 = float4(1, 0, 1, 1) * input.light;
    float4 color2 = float4(0, 1, 0, 1) * input.light;
    float4 finalColor = color1 * (fmod(floor(input.originalPos.x / 6.0f), 5.0f) / 5.0f) + color2 * (1 - fmod(floor(input.originalPos.x / 6.0f), 5.0f) / 5.0f);
    
    return finalColor;
}
//--------------------------------------------------------------------------------------
//Mirror reflecting cubemap
struct VS_OUTPUT_MIRROR
{
    float4 Pos : SV_POSITION;
    float4 Norm : TEXCOORD0;
    float4 cameraPos : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};
VS_OUTPUT_MIRROR VS_mirror(float4 Pos : POSITIONT, float3 N : NORMAL)
{
    VS_OUTPUT_MIRROR output = (VS_OUTPUT_MIRROR) 0;
    output.Pos = Pos;
    output.Pos = mul(output.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    output.Norm = normalize(mul(float4(N, 0.0f), World));
    output.cameraPos = cameraPos;
    
    float4 worldPos = mul(float4(Pos.xyz, 1), World);
    output.viewDir = normalize(cameraPos.xyz - worldPos.xyz);
    return output;
}
float4 PS_mirror(VS_OUTPUT_MIRROR input) : SV_Target
{
    float3 viewDir = reflect(-input.viewDir, input.Norm.xyz);
    float3 refrDir = refract(-input.viewDir, input.Norm.xyz, 0.9);
    float4 skyColor = txSkyColor.Sample(txSkySampler, viewDir);
    float4 refrColor = txSkyColor.Sample(txSkySampler, refrDir);
    return 0.5 * skyColor + 0.5 * refrColor;
}
//--------------------------------------------------------------------------------------
//particle system for fire
struct VS_OUTPUT_PARTICLE
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float4 PosProj : TEXCOORD1;
};
VS_OUTPUT_PARTICLE VS_particle(float4 Pos : POSITION)
{
    VS_OUTPUT_PARTICLE output = (VS_OUTPUT_PARTICLE) 0;
    output.Pos = Pos;

    float4 mathsPos = output.Pos;
    
    //translate such that the middle plane is in the center of the object
    output.Pos.z += 45.5;
    //compress into middle plane
    output.Pos.z *= 1 / 50;
    //move to death plane
    output.Pos.y -= 130;
    output.PosProj = output.Pos;
    //billboarding
    //transform according to View
    float3 viewRight = View._11_21_31;
    float3 viewUp = View._12_22_32;
    float3 viewForward = View._13_23_33;
    output.Pos.xyz = output.Pos.x * viewRight + output.Pos.y * viewUp + output.Pos.z * viewForward;
    output.Pos.w = 1.0f;
    //TRANSLATE according to World but do not allow rotation or scaling
    output.Pos.xyz += World._41_42_43;
 
    //finish billboarding
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    const float rng1 = hash(Pos.z);
    //RNG is used to set initial x position of each particle
    //15.0f from magnification
    output.Pos.x += rng1 * 15.0f;
    
    const float rng2 = hash(rng1);
    //RNG is used to set lifespan of each particle
    const float lifespan = 0.8 + rng2 / 8.0;

    const float rng3 = hash(rng2);
    //RNG is used to set velocity of each particle
    const float3 velocity = 2.0f * float3(0, rng3 / 2.0f + 3, 0);
    
    const float rng4 = hash(rng3);
    //RNG is used to set the initial time of each particle
    float t = time + rng4 + 1;

    //15.0f from magnification
    float3 particlePos = velocity * fmod(t, lifespan) * 15.0f;
    output.Pos.xyz += particlePos;
    //Particles get more yellow as they rise
    //15.0f from magnification
    output.Color = float4(0.8, particlePos.y * 0.2 / 15.0f, 0, 0.18);
    
    return output;
}
float4 PS_particle(VS_OUTPUT_PARTICLE input) : SV_Target
{
    return input.Color;
}
//--------------------------------------------------------------------------------------
//textures
Texture2D cliffTexture : register(t1);
Texture2D grassTexture : register(t2);
Texture2D steelTexture : register(t3);
Texture2D brushedSteelTexture : register(t4);

float4 PS_texture(VS_OUTPUT input) : SV_Target
{
    float4 myTexture = float4(0, 0, 0, 0);
    if (input.texType == 0)
    {
        myTexture = cliffTexture.Sample(basicSampler, input.Tex);
    }
    else if (input.texType == 1)
    {
        myTexture = grassTexture.Sample(basicSampler, input.Tex);
    }
    else if (input.texType == 2)
    {
        myTexture = steelTexture.Sample(basicSampler, input.Tex);
    }
    else if (input.texType == 3)
    {
        myTexture = brushedSteelTexture.Sample(basicSampler, input.Tex);
    }
    myTexture.xyz *= 0.5;
    float4 lighting = calculateLighting(input.spotlight, input.Norm, input.lightPos.xyz, float4(input.PosWorld, 1), input.PosWorld, input.cameraPos, true);
    myTexture.xyz += lighting.xyz;
    return myTexture;
}
//--------------------------------------------------------------------------------------
float4 calculateLighting(float spotlight, float3 normal, float3 sunPos, float4 finalPos, float3 posWorld, float4 myCamera, bool texturedObject)
{
    float4 materialAmb = float4(0, 0, 0, 1.0);
    float4 materialSpec = float4(1.0, 1.0, 1.0, 1.0);
    float4 lightCol = float4(1.0, 1.0, 1.0, 1.0);
    
    //Textured objects do not use specular light and ambient light is calculated outside of this function
    if (texturedObject)
    {
        materialSpec = float4(0, 0, 0, 1);
    }
    //set colors of nontextured 2D objects
    //clouds
    else if (posWorld.y >= 400)
    {
        materialAmb = float4(0.9, 0.9, 0.9, 1.0);
    }
    //upper tree branches
    else if (posWorld.y <= 196.37 && posWorld.y >= 160)
    {
        const float4 leafGreen = float4((10 * 16 + 2) / 255.0f, (11 * 16) / 255.0f, (6 * 16 + 14) / 255.0f, 1.0f);
        materialAmb = leafGreen;
    }
    //tree trunks
    else if (posWorld.y <= 160 && posWorld.y >= 100)
    {
        const float4 woodBrown = float4((7 * 16 + 2) / 255.0f, (5 * 16 + 12) / 255.0f, (4 * 16 + 2) / 255.0f, 1.0f);
        materialAmb = woodBrown;
    }
    //foreground tree branches
    else if (posWorld.y <= -33.63 && posWorld.y >= -70)
    {
        const float4 leafGreen = float4((10 * 16 + 2) / 255.0f, (11 * 16) / 255.0f, (6 * 16 + 14) / 255.0f, 1.0f);
        materialAmb = leafGreen;
    }
    //foreground tree trunks
    else if (posWorld.y <= -70 && posWorld.y >= -130)
    {
        const float4 woodBrown = float4((7 * 16 + 2) / 255.0f, (5 * 16 + 12) / 255.0f, (4 * 16 + 2) / 255.0f, 1.0f);
        materialAmb = woodBrown;
    }
    //collectables
    else
    {
        //this is my favorite color but unfortunately it does not look right here
        const float4 darkCerulean = float4(0, (5 * 16 + 6) / 255.0f, (8 * 16 + 2) / 255.0f, 1.0f);
        materialAmb = darkCerulean;
    }
    float4 materialDiff;
    if (texturedObject)
        materialDiff = float4(0.5, 0.5, 0.5,1);
    else
        materialDiff = materialAmb;
   
    float3 V = normalize(posWorld - myCamera.xyz);
    float f = 200.0f;
    if (spotlight == 0)
    {
        //sun calculation
        float3 lightDir = normalize(sunPos.xyz - finalPos.xyz);
        float diff = max(0.0, dot(lightDir, normal));
        //sun specular
        float3 R = normalize(reflect(lightDir, normal));
        float spec = pow(max(0.0, dot(V, R)), f);
        return float4((materialAmb + diff * materialDiff + spec * materialSpec) * lightCol);
    }
    else
    {
        //spotlight calculation
        float3 lightDir = normalize(myCamera.xyz - finalPos.xyz);
        float diff;
        //spotlight specular
        float3 R = normalize(reflect(myCamera.xyz, normal));
        float spec;
        float spotLightSize = 20.0f * (distance(myCamera.xyz, posWorld) / 80.0f);
        if (sqrt(pow(myCamera.x - posWorld.x, 2.0) + pow(myCamera.y - posWorld.y-12, 2.0)) <= spotLightSize)
        {
            diff = max(0.0, dot(lightDir, normal));
        //spotlight specular
            spec = pow(max(0.0, dot(V, R)), f);
        }
        else
        {
            diff = 0;
            spec = 0;
        }
        return float4((materialAmb + diff * materialDiff + spec * materialSpec) * lightCol);
    }
}