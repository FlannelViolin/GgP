
#pragma once
#include "DirectXGame.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include <fstream>
#include <vector>

using namespace DirectX;
class Mesh
{
public:
	Mesh(Vertex* vertexArray, int numVerts, u_int* indexArray, int numIndices, ID3D11Device* devObj);
	Mesh(char* fileName,ID3D11Device* devObj);


	Mesh();
	~Mesh();

	ID3D11Buffer* Mesh::getVertexBuffer();
	ID3D11Buffer* Mesh::getIndexBuffer();
	int Mesh::getIndexCount();
//variables
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	//char* fileName;

	// integer specifying how many indices are in the mesh's index buffer
	unsigned int indices;

};

