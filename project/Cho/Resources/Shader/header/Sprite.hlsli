
struct World
{
    float4x4 World;
};

struct SpriteVertexShaderOutput
{
    float2 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};