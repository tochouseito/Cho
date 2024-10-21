struct ViewProjection
{
    float4x4 view;
    float4x4 projection;
    float4x4 matWorld;
    float3 cameraPosition;
};

struct Transform
{
    float4x4 matWorld;
    float4x4 worldInverse;
    float4x4 rootNode;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
    float3 cameraPosition : CAMERAPOS0;
};