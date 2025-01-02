#include"header/Sprite.hlsli"

ConstantBuffer<World> gWorld : register(b0);

struct SpriteVertexShaderInput
{
    float2 position : POSITION;
    float2 texcoord : TEXCOORD0;
};
SpriteVertexShaderOutput main(SpriteVertexShaderInput input)
{
    SpriteVertexShaderOutput output;
    float4 position = input.position;
    output.position = mul(position, gWorld.World);
    output.texcoord = input.texcoord;
    
    return output;
}