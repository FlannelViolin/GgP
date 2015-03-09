#include "Camera.h"


Camera::Camera(XMFLOAT4X4 v, XMFLOAT4X4 proj, XMFLOAT3 pos){
	view = v;
	projection = proj;
	position = pos;
	//direction is facing forward
	direction = XMFLOAT3(0, 0, 1);
	xRot = 0.0f;
	yRot = 0.0f;
	moveIncrement = .001f;
}


Camera::~Camera()
{
}

void Camera::Update(){

	////Update the look at of the camera....
	XMVECTOR tempDirection = XMLoadFloat3(&direction);
	XMVECTOR leftVector = XMVectorSet(0,0,0,0);
	XMVECTOR rightVector = XMVectorSet(0, 0, 0, 0);
	XMVECTOR tempZunit;
	XMVECTOR tempYunit;
	XMVECTOR pos;
	// unit vector in z direction? sure
	tempZunit = XMVectorSet(0, 0, 1, 0);
	tempYunit = XMVectorSet(0, 1, 0, 0);
	

	// get quaternion rotation
	XMVECTOR rotations = XMQuaternionRotationRollPitchYaw(xRot, yRot, 0.0f);
	rotations = XMQuaternionNormalize(rotations);

	// apply quarternion 
	XMVECTOR afterRot = XMVector3Rotate(tempZunit, rotations);

	// load position
	pos = XMLoadFloat3(&position);

	// store new direction
	XMStoreFloat3(&direction, afterRot);


	// store in view matrix
	XMMATRIX storeMe  = XMMatrixLookToLH(pos, afterRot, tempYunit);
	XMStoreFloat4x4(&view, XMMatrixTranspose(storeMe));
	

	/////input....
	//forward
	if (GetAsyncKeyState('W') & 0x8000) { 
		position.x += direction.x*moveIncrement;
		position.y += direction.y*moveIncrement;
		position.z += direction.z*moveIncrement;
	 }
	//left
	if (GetAsyncKeyState('A') & 0x8000) { 
		leftVector = XMVector3Cross(afterRot, tempYunit);
		position.x += XMVectorGetIntX(leftVector)*moveIncrement;
		position.y += XMVectorGetIntY(leftVector)*moveIncrement;
		position.z += XMVectorGetIntZ(leftVector)*moveIncrement;
	}
	// backwards
	if (GetAsyncKeyState('S') & 0x8000) { 
		position.x -= direction.x*moveIncrement;
		position.y -= direction.y*moveIncrement;
		position.z -= direction.z*moveIncrement;
	}
	// right
	if (GetAsyncKeyState('D') & 0x8000) { 
		rightVector = -leftVector;
		position.x += XMVectorGetIntX(rightVector)*moveIncrement;
		position.y += XMVectorGetIntY(rightVector)*moveIncrement;
		position.z += XMVectorGetIntZ(rightVector)*moveIncrement;
	}
	// up
	if (GetAsyncKeyState('X') & 0x8000) {
		position.y += moveIncrement;
	}
	// down 
	if (GetAsyncKeyState('Z') & 0x8000) {
		position.y -= moveIncrement;
	}
	
}


XMFLOAT4X4 Camera::getProjectionMatrix(){ return projection; }
XMFLOAT4X4 Camera::getViewMatrix(){ return view; }


// I have for seriousness no Idea what I'm doing
void Camera::rotateX(float diff){
	xRot += diff;
}


void Camera::rotateY(float diff){
	yRot += diff;
}

void Camera::updateProjectionMatrix(XMFLOAT4X4 newProj){
	projection = newProj;
}