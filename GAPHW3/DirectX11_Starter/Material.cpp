#include "Material.h"


Material::Material(SimplePixelShader* p, SimpleVertexShader* v, ID3D11ShaderResourceView* sv, ID3D11SamplerState* ss)
{
	pShade = p;
	vShade = v;
	shaderView = sv;
	sampleState = ss;
}


Material::~Material()
{
}


SimplePixelShader* Material::getPixelShader(){
	return pShade;
}
SimpleVertexShader* Material::getVertexShader(){
	return vShade;
}

ID3D11ShaderResourceView* Material::getShaderView(){
	return shaderView;
}
ID3D11SamplerState* Material::getSampleState(){
	return sampleState;
}

