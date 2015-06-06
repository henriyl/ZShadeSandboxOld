//#include "stdafx.h"
#include "PlaneEnvironment.h"
//===============================================================================================================================
//===============================================================================================================================
PlaneEnvironment::PlaneEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   Environment3D(base_window_name, render_window_name, eo, init_lua)
{
	/*
	mShader = new PosNormalTexTanShader(mD3DSystem, "Basic\\ZShadeSandboxMesh::VertexNormalTexTan.fxo");

	//
	// Testing Mesh for the environment
	//

	//D3DX11CreateShaderResourceViewFromFile(mD3DSystem->GetDevice11(), "Textures\\floor.dds", 0, 0, &mColorTexture, 0);
	//D3DX11CreateShaderResourceViewFromFile(mD3DSystem->GetDevice11(), "Textures\\floor_nmap.dds", 0, 0, &mNormalTexture, 0);

	GeometryGenerator::MeshData plane;
	GeometryGenerator gGen;

	gGen.CreateGrid(300.0f, 10.0f, 300.0f, 100.0f, 100.0f, plane);

	vector<ZShadeSandboxMesh::VertexNormalTexTan> vertices(plane.Vertices.size());

	UINT k = 0;
	for(size_t i = 0; i < plane.Vertices.size(); ++i, ++k)
	{
		vertices[k].position      = plane.Vertices[i].position;
		vertices[k].normal   = plane.Vertices[i].Normal;
		vertices[k].texture      = plane.Vertices[i].TexC;
		vertices[k].tangentU = plane.Vertices[i].TangentU;
	}

	//Build the buffers
	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTexTan) * vertices.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the vertex data
	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = mD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &mVB);
	if (FAILED(result)) return;

	//
	// Create Indices
	//

	mIndexCount = plane.Indices.size();

	vector<UINT> indices;

	indices.insert( indices.end(), plane.Indices.begin(), plane.Indices.end() );

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = mD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &mIB);
	if (FAILED(result)) return;
	*/
}
//===============================================================================================================================
PlaneEnvironment::~PlaneEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool PlaneEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void PlaneEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void PlaneEnvironment::Update()
{
	
}
//===============================================================================================================================
void PlaneEnvironment::Render()
{
	//unsigned int stride;
	//unsigned int offset;

	//Set the vertex buffer stride and offset
	//stride = sizeof(ZShadeSandboxMesh::VertexNormalTexTan);
	//offset = 0;

	//mD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	//mD3DSystem->GetDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer
	//mD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//
	// Render the plane with the shader
	//
	/*
	mShader->SetWorld( WorldXM() );
	mShader->SetViewProj( mCamera->ViewProj() );

	mShader->SetLightDirection( mLight1.mDirection );
	mShader->SetLightAmbientIntensity( 0.2f );
	mShader->SetLightAmbientColor( mLight1.mAmbientColor );
	mShader->SetLightDiffuseIntensity( 0.2f );
	mShader->SetLightDiffuseColor( mLight1.mDiffuseColor );
	mShader->SetLightSpecularColor( mLight1.mSpecularColor );
	mShader->SetSpecularShininess( 300.0f );
	mShader->SetEyeposition( mCamera->position() );

	mShader->SetColorTexture( mColorTexture );
	mShader->SetNormalTexture( mNormalTexture );
	*/

	//mShader->EnableLighting( mEnableLighting );

	//mShader->Render11(mD3DSystem, mIndexCount);
}
//===============================================================================================================================
void PlaneEnvironment::RenderDeferred()
{

}
//===============================================================================================================================