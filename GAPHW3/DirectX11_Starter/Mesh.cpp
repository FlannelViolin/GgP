#include "Mesh.h"

// where do we use numVerts?
Mesh::Mesh(Vertex* vertexArray, int numVerts, u_int* indexArray, int numIndices, ID3D11Device* devObj){

	indices = numIndices;
		

	// Create the vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts; // Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertexArray;
	HR(devObj->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));

	// Create the index buffer
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * numIndices; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indexArray;
	HR(devObj->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));

}


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);

}

ID3D11Buffer* Mesh::getVertexBuffer(){
	return vertexBuffer;
}
ID3D11Buffer* Mesh::getIndexBuffer(){
	return indexBuffer;

}
int Mesh::getIndexCount(){
	return indices;
}