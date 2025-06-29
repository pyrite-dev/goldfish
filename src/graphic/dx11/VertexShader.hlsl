cbuffer MatrixBuffer : register(b0) {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_INPUT {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    
    float4 worldPos = mul(float4(input.pos, 1.0f), worldMatrix);
    float4 viewPos = mul(worldPos, viewMatrix);
    output.pos = mul(viewPos, projectionMatrix);
    
    output.color = input.color;
    return output;
}
