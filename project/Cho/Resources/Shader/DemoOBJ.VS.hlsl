


float4 main( float4 pos : POSITION ) : SV_POSITION
{
    float4x4 mat =
    {
        1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0
    };

    float4x4 invMat = inverse(mat);
    return pos;
}