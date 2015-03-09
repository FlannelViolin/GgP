#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
class Material
{

private: 
	ID3D11PixelShader* pShade;
	ID3D11VertexShader* vShade;
public:

	ID3D11PixelShader* getPixelShader();
	ID3D11VertexShader* getVertexShader();
	Material(ID3D11PixelShader* p, ID3D11VertexShader* v);
	~Material();
};

