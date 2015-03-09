#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
class Camera
{

private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	float xRot; 
	float yRot;
	float moveIncrement;

public:
	Camera(XMFLOAT4X4 view, XMFLOAT4X4 proj, XMFLOAT3 pos);
	Camera(XMVECTOR position, XMVECTOR direction, float aspectRatio);
	~Camera();

	void Update();

	XMFLOAT4X4 getViewMatrix();
	XMFLOAT4X4 getProjectionMatrix();

	void rotateX(float diff);
	void rotateY(float diff);

	void updateProjectionMatrix(float aspectRatio);
};

