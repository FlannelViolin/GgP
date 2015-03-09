#include "Entity.h"



Entity::Entity()
{
}

Entity::Entity(Mesh * m, XMFLOAT3 p, XMFLOAT3 r, XMFLOAT3 s){
	mesh = m;
	pos = p;
	rot = r;
	scale = s;

	/*XMVECTOR posVector = XMLoadFloat3(&pos);
	XMVECTOR scaleVector = XMLoadFloat3(&scale);
	XMVECTOR rotVector = XMLoadFloat3(&rot);
	
	XMMATRIX transMatrix = XMMatrixTranslation(pos.x,pos.y,pos.z);

	XMMATRIX rotMatrixX = XMMatrixRotationX(rot.x);
	XMMATRIX rotMatrixY = XMMatrixRotationY(rot.y);
	XMMATRIX rotMatrixZ = XMMatrixRotationZ(rot.z);

	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX rotMatrix = rotMatrixX * rotMatrixY * rotMatrixZ;

	XMMATRIX w = scaleMatrix * rotMatrix * transMatrix;
	*/
	// just kidding w is the indentity matrix
	XMMATRIX w = XMMatrixIdentity();

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(w));
	
	

}

Entity::Entity(Mesh * m){
	mesh = m;
	XMMATRIX w = XMMatrixIdentity();


	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(w));
}

Entity::~Entity()
{
	delete(mesh);

	
}


XMFLOAT4X4 Entity::getWorldMatrix(){ return worldMatrix; }
XMFLOAT3 Entity::getPosition(){	return pos;}
XMFLOAT3 Entity::getRotation(){ return rot; }
XMFLOAT3 Entity::getScale(){ return scale; }



void Entity::setWorldMatrix(XMFLOAT4X4  wm){ worldMatrix = wm; }
void Entity::setPosition(XMFLOAT3 p){ pos = p; }
void Entity::setRotation(XMFLOAT3 r){ rot = r; }
void Entity::setScale(XMFLOAT3 s){ scale = s; }

void Entity::setPosition(float x, float y, float z){ pos.x = x; pos.y = y; pos.z = z; }
void Entity::setRotation(float x, float y, float z){ rot.x = x; rot.y = y; rot.z = z; }
void Entity::setScale(float x, float y, float z){ scale.x = x; scale.y = y; scale.z = z; }


void Entity::setMaterial(Material* mat){
	material = mat;
}



boolean Entity::updateWorldMatrix(){
	XMMATRIX transMatrix = XMMatrixTranslation(pos.x, pos.y, pos.z);

	XMMATRIX rotMatrixX = XMMatrixRotationX(rot.x);
	XMMATRIX rotMatrixY = XMMatrixRotationY(rot.y);
	XMMATRIX rotMatrixZ = XMMatrixRotationZ(rot.z);

	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX rotMatrix = rotMatrixX * rotMatrixY * rotMatrixZ;

	XMMATRIX w = scaleMatrix * rotMatrix * transMatrix;
	
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(w));

	return true;
}

boolean Entity::Draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectMatrix){
	
	updateWorldMatrix();


	material->getVertexShader()->SetFloat3("position", pos);
	material->getVertexShader()->SetMatrix4x4("world", worldMatrix);
	material->getVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->getVertexShader()->SetMatrix4x4("projection", projectMatrix);

	material->getVertexShader()->SetShader();
	/*ctx->UpdateSubresource(
		vsConstantBuffer,
		0,
		NULL,
		&data,
		0,
		0);

	// Set the constant buffer to be used by the Vertex Shader
	//  - This should be done PER OBJECT you intend to draw, as each object  
	//    will probably have different data to send to the shader (matrices
	//    in this case)
	ctx->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&vsConstantBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer = mesh->getVertexBuffer();
	ID3D11Buffer* indexBuffer = mesh->getIndexBuffer();
	ctx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	ctx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	ctx->DrawIndexed(
		mesh->getIndexCount(),	// The number of indices we're using in this draw
		0,
		0);*/

	return true;
}