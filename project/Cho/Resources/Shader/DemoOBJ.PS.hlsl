#include"header/Demo.hlsli"

// テクスチャリソース(カラー)
Texture2D<float4> gTexture : register(t0);

// サンプラー
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 pixel : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // テクスチャ
    //float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    
    output.pixel = textureColor;

    return output;
}