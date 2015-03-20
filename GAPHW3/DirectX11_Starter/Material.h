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

	ID3D11ShaderResourceView*	shaderView;
	ID3D11SamplerState*			sampleState;


public:

	SimplePixelShader* getPixelShader();
	SimpleVertexShader* getVertexShader();

	ID3D11ShaderResourceView* getShaderView();
	ID3D11SamplerState* getSampleState();


	Material(SimplePixelShader* p, SimpleVertexShader* v, ID3D11ShaderResourceView* sv, ID3D11SamplerState* ss);
	~Material();
};

