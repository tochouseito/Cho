struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};
struct GPUParticle
{
    float3 translate;
    float3 rotate;
    float3 scale;
    float lifeTime;
    float3 velocity;
    float currentTime;
    float4 color;
    int isAlive;
};
struct PerView
{
    float4x4 View;
    float4x4 Projection;
    //float32_t4x4 billboardMatrix;
    float4x4 cameraMat;
    float3 cameraPosition;
};
struct PerFrame
{
    float time;
    float deltaTime;
};
struct EmitterSphere
{
    float3 translate;
    float radius;
    uint count;
    float frequency;
    float frequencyTime;
    uint emit;
};
