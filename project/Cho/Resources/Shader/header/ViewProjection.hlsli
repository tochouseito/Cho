struct ViewProjection
{
    float4x4 view;
    float4x4 projection;
    float4x4 matWorld;
    float4x4 matBillboard;
    float3 cameraPosition;
};