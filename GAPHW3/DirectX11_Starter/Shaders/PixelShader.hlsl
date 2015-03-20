
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;

};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer lightBuffer :register(b0){
	DirectionalLight light1;
	DirectionalLight light2;
}

float4 lightCalc(float3 normal, DirectionalLight l){
	
	float4 diffuseColor = l.DiffuseColor;
	float3 nDirectionTO = normalize(-l.Direction);
	float lightAmount = saturate(dot(normal, nDirectionTO));

	diffuseColor = diffuseColor*lightAmount;
	float4 totalColor = diffuseColor + l.AmbientColor;
		// Just return the input color
		// - Note that this color (like all values that pass through the rasterizer)
		//   is interpolated for each pixel between the corresponding 
		//   vertices of the triangle
		return float4(totalColor);
}

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);
// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float4 lightOutput = lightCalc(input.normal, light1);// +lightCalc(input.normal, light2);
		float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
		return lightOutput *surfaceColor;
}

