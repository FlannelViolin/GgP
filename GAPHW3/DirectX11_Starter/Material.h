#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleShader.h"
class Material
{

private: 
	SimplePixelShader* pShade;
	SimpleVertexShader* vShade;
	void updateShaders();
public:

	SimplePixelShader* getPixelShader();
	SimpleVertexShader* getVertexShader();
	
	Material(SimplePixelShader* p, SimpleVertexShader* v);
	~Material();
};

