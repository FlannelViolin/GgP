#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
using namespace DirectX;

struct VertexShaderConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

class Entity
{
private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 pos, rot, scale;
	Mesh* mesh;
	Material* mat;
	//ID3D11Buffer* vsConstantBuffer;
	VertexShaderConstantBufferLayout data;

public:
	Entity(Mesh * m, XMFLOAT3 p, XMFLOAT3 r, XMFLOAT3 s);
	Entity(Mesh *m);
	XMFLOAT4X4 getWorldMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();



	void setWorldMatrix(XMFLOAT4X4 wm);
	void setPosition(XMFLOAT3 p);
	void setRotation(XMFLOAT3 r);
	void setScale(XMFLOAT3 s);
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	// pass in values, and positions, id love to pass in a reference to the vector to be changed.
	void changePositionValues(float* values, int* positions);
	void changeRotationValues(float* values, int* positions); 
	void changeScaleValues(float* values, int* positions);

	// test this, pass in a vector with a getter value, 
	void changeVectorValues(XMFLOAT3& vect, float* values, int* positions);

	boolean updateWorldMatrix();

	boolean Draw(ID3D11DeviceContext* ctx, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectMatrix, ID3D11Buffer* vsConstantBuffer);
		 
		
	Entity();
	~Entity();
};

