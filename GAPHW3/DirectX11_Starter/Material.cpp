#include "Material.h"


Material::Material(ID3D11PixelShader* p, ID3D11VertexShader* v)
{
	pShade = p;
	vShade = v;
}


Material::~Material()
{
}


ID3D11PixelShader* Material::getPixelShader(){
	return pShade;
}
ID3D11VertexShader* Material::getVertexShader(){
	return vShade;
}