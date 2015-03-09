#include "Material.h"


Material::Material(SimplePixelShader* p, SimpleVertexShader* v)
{
	pShade = p;
	vShade = v;
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