// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);
	
	// If we can't initialize, we can't run
	if( !game.Init() )
		return 0;
	
	// All set to run the game
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Demo DX11 Game";
	windowWidth = 800;
	windowHeight = 600;
}

MyDemoGame::~MyDemoGame()
{
	// Release all of the D3D stuff that's still hanging out
	/*ReleaseMacro(vertexBuffer);
	ReleaseMacro(indexBuffer);*/
	/*ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(vsConstantBuffer);
	ReleaseMacro(inputLayout);*/
	delete(mesh1);
	delete(mesh2);
	delete(mesh3);
	delete(Entity1);
	delete(Entity2);
	delete(Entity3);
	delete(camera);
	timer.Stop();

}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	// Make sure DirectX initializes properly
	if( !DirectXGame::Init() )
		return false;

	// Create the necessary DirectX buffers to draw something
	CreateGeometryBuffers();

	// Load pixel & vertex shaders, and then create an input layout
	LoadShadersAndInputLayout();

	// Set up camera-related matrices
	InitializeCameraMatrices();

	// Set up world matrix
	// In an actual game, each object will need one of these and they should
	//  update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	// Successfully initialized
	return true;
}

// Creates the vertex and index buffers for a single triangle
void MyDemoGame::CreateGeometryBuffers()

{

	timer.Start();
	// Create some temporary variables to represent colors
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices we want to put into the Vertex Buffer
	Vertex vertices1[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f),XMFLOAT3(0,0,-1),XMFLOAT2(0,0) },
		{ XMFLOAT3(-.75f, 0.0f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },
		{ XMFLOAT3(.75f, 0.0f, +0.0f), XMFLOAT3(0, 0, -1), XMFLOAT2(0, 0) },

	};
	/*Vertex vertices2[] = 
	{
		{ XMFLOAT3(-.75f, 0.0f, +0.0f), green },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), blue },
		{ XMFLOAT3(+0.0f, -1.0f, +0.0f), red },

	};
	Vertex vertices3[] =
	{
		{ XMFLOAT3(.75f, 0.0f, +0.0f),blue },
		{ XMFLOAT3(0.0f, -1.0f, +0.0f), red },
		{ XMFLOAT3(+1.5f, -1.0f, +0.0f), green },

	};*/

	// Set up the indices of the vertices (necessary for indexed drawing)
	UINT indices1[] = { 0, 2, 1 };
	UINT indices2[] = { 3, 5, 4 };
	UINT indices3[] = { 6, 8, 7 };

	// what do we pass in for the numbers?
	mesh1 = new Mesh(vertices1, 3, indices1, 3, device);
	/*mesh2 = new Mesh(vertices2, 3, indices1, 3, device);
	mesh3 = new Mesh(vertices3, 3, indices1, 3, device);*/

	XMFLOAT3 ones = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMFLOAT3 zeroes = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 tryScale = XMFLOAT3(1.5f, 1.5f, 0.0f);

	Entity1 = new Entity(mesh1, zeroes,zeroes, ones);

	pShader = new SimplePixelShader(device, deviceContext);

	vShader = new SimpleVertexShader(device, deviceContext);

	pShader->LoadShaderFile(L"../Debug/PixelShader.cso");

	vShader->LoadShaderFile(L"../Debug/VertexShader.cso");

	Entity1->setMaterial(new Material(pShader, vShader));

	/*
	Entity2 = new Entity(mesh2, zeroes, zeroes, ones);
	Entity3 = new Entity(mesh1, zeroes, zeroes, ones);*/

	
}

// Loads shaders from compiled shader object (.cso) files, and uses the
// vertex shader to create an input layout which is needed when sending
// vertex data to the device
void MyDemoGame::LoadShadersAndInputLayout()
{




	// OLD SHADER
	// Set up the vertex layout description
	// This has to match the vertex input layout in the vertex shader
	// We can't set up the input layout yet since we need the actual vert shader
	/*D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0}
	};*/

	// Load Vertex Shader --------------------------------------
	/*ID3DBlob* vsBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &vsBlob);

	// Create the shader on the device
	HR(device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Clean up
	ReleaseMacro(vsBlob);

	// Load Pixel Shader ---------------------------------------
	ID3DBlob* psBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &psBlob);

	// Create the shader on the device
	HR(device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
		&pixelShader));

	// Clean up
	ReleaseMacro(psBlob);

	// Constant buffers ----------------------------------------
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth           = sizeof(dataToSendToVSConstantBuffer);
	cBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags		= 0;
	cBufferDesc.MiscFlags			= 0;
	cBufferDesc.StructureByteStride = 0;
	HR(device->CreateBuffer(
		&cBufferDesc,
		NULL,
		&vsConstantBuffer));*/
}

// Initializes the matrices necessary to represent our 3D camera
void MyDemoGame::InitializeCameraMatrices()
{
	// Create the View matrix
	// In an actual game, update this when the camera moves (every frame)
	XMVECTOR position = XMVectorSet(0, 0, -5, 0);
	//XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR direction = XMVectorSet(0, 0, 1, 0);
	//XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	//XMMATRIX V = XMMatrixLookToLH(position, direction, up); // View matrix creation:
	// Looks at a "target" from
	// a particular "position"
	//XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	// Create the Projection matrix
	// This should match the window's aspect ratio, and also update anytime
	// the window resizes (which is already happening in OnResize() below)
	/*XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		AspectRatio(),				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
	XMStoreFloat3(&cameraPos, position); */


	//camera = new Camera(viewMatrix, projectionMatrix, cameraPos);
	camera = new Camera(position, direction, AspectRatio());
}

#pragma endregion

#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	 //Update our projection matrix since the window size changed
	/*XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		AspectRatio(),
		0.1f,
		100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));*/
	if (camera != NULL) camera->updateProjectionMatrix(AspectRatio());
}
#pragma endregion

#pragma region Game Loop

// Update your game state
void MyDemoGame::UpdateScene(float dt)
{
	// Take input, update game logic, etc.
	if (camera!=NULL) camera->Update();
}

// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	timer.Tick();
	// Background color (PURPLE in this case) for clearing 
	const float color[4] = {0.4f, .1f, 0.75f, 0.0f};

	// Clear the buffer (erases what's on the screen)
	//  - Do this once per frame
	//  - At the beginning (before drawing anything)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// OLD SHADER
	// Set up the input assembler
	//  - These technically don't need to be set every frame, unless you're changing the
	//    input layout (different kinds of vertices) or the topology (different primitives)
	//    between draws
	//deviceContext->IASetInputLayout(inputLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// Set the current vertex and pixel shaders
	//  - These don't need to be set every frame YET
	//  - Once you have multiple shaders, you will need to change these
	//    between drawing objects that will use different shaders
/*	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);*/
	

	XMFLOAT3 changePos = Entity1->getPosition();
	changePos.y = sin(timer.TotalTime());
	Entity1->setPosition(changePos);

	/*vShader->SetFloat3("position", Entity1->getPosition());
	vShader->SetMatrix4x4("world", worldMatrix);
	vShader->SetMatrix4x4("view", viewMatrix);

	vShader->SetShader(true);
	*/
	Entity1->Draw(camera->getViewMatrix(), camera->getProjectionMatrix(),deviceContext);






	HR(swapChain->Present(0, 0));
}

/*void MyDemoGame::drawObject(Mesh* mesh){
	// Copy CPU-side data to a single CPU-side structure  
	//  - Allows us to send the data to the GPU buffer in one step
	//  - Do this PER OBJECT, before drawing it
	dataToSendToVSConstantBuffer.world = worldMatrix;
	dataToSendToVSConstantBuffer.view = viewMatrix;
	dataToSendToVSConstantBuffer.projection = projectionMatrix;

	// Update the GPU-side constant buffer with our single CPU-side structure  
	//  - Faster than setting individual sub-variables multiple times
	//  - Do this PER OBJECT, before drawing it
	deviceContext->UpdateSubresource(
		vsConstantBuffer,
		0,
		NULL,
		&dataToSendToVSConstantBuffer,
		0,
		0);

	// Set the constant buffer to be used by the Vertex Shader
	//  - This should be done PER OBJECT you intend to draw, as each object  
	//    will probably have different data to send to the shader (matrices
	//    in this case)
	deviceContext->VSSetConstantBuffers(
		0,	// Corresponds to the constant buffer's register in the vertex shader
		1,
		&vsConstantBuffer);

	// Set buffers in the input assembler
	//  - This should be done PER OBJECT you intend to draw, as each object could
	//    potentially have different geometry (and therefore different buffers!)
	//  - You must have both a vertex and index buffer set to draw
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer = mesh->getVertexBuffer();
	ID3D11Buffer* indexBuffer = mesh->getIndexBuffer();
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(
		mesh->getIndexCount(),	// The number of indices we're using in this draw
		0,
		0);
}*/

#pragma endregion

#pragma region Mouse Input

// These methods don't do much currently, but can be used for mouse-related input

void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	camera->rotateX(y - prevMousePos.y);
	camera->rotateY(x - prevMousePos.x);
	prevMousePos.x = x;
	prevMousePos.y = y;
	
	SetCapture(hMainWnd);
}

void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{

	prevMousePos.x = x;
	prevMousePos.y = y;
}
#pragma endregion