//***************************************************************************************
// color.hlsl by Frank Luna (C) 2015 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj; 
};

struct VertexIn
{
	float3 PosL  : POSITION;
	float2 Tex   : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float3 world_norm = normalize(vin.Normal);
	float3 light_dir = normalize(float3(-0.25f,0.25f,-0.25f));
	float3 diffuse = max(0,dot(light_dir,world_norm));
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = float4(float3(0.5f,0.5f,0.5f)+diffuse*float3(1.0f,1.0f,1.0f),1.0f);
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color;
}


