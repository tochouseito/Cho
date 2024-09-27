#include"header/obj3d.hlsli"

struct Material
{
    float4 colorMaterial : SV_TARGET0;
    int enableLighting;
    float4x4 uvTransform;
    float shininess;
};
ConstantBuffer<Material> gMaterial : register(b0);

struct PixelShaderOutput
{
    float4 colorPS : SV_TARGET0;
};

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // カメラへの方向を算出
    //float3 toEye = normalize(input.cameraPosition - input.worldPosition);
    
    
   // float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    //float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    // textureのa値が0のときにPixelを棄却
    //if (textureColor.a <= 0.5)
    //{
    //    discard;
    //}
    //if (textureColor.a == 0.0)
    //{
    //    discard;
    //}
    
    //output.colorPS = gMaterial.colorMaterial * textureColor;
    //output.colorPS.a = 1.0f;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    output.colorPS = gMaterial.colorMaterial * textureColor;
    return output;
}