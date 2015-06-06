#include "Shader.h"
#include "CGlobal.h"
#include <sstream>
using ZShadeSandboxShader::Shader;
//=================================================================================================================
//=================================================================================================================
Shader::Shader(D3D* d3d)
:	m_pVertexShader(0), m_pHullShader(0), m_pDomainShader(0),
	m_pPixelShader(0), m_pGeometryShader(0), m_pComputeShader(0),
    m_Wireframe(false), m_pD3DSystem(d3d), m_UseInputLayout(true),
	m_UseOrtho(false), m_UseCustomWorld(false)
{
}
//=================================================================================================================
Shader::Shader(const Shader& c)
{
}
//=================================================================================================================
Shader::~Shader()
{
	/*SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pHullShader);
	SAFE_RELEASE(m_pDomainShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pGeometryShader);
	SAFE_RELEASE(m_pComputeShader);

	map<BetterString, ID3D11VertexShader*>::iterator   VertexShadersIter;
	map<BetterString, ID3D11PixelShader*>::iterator    PixelShadersIter;
	map<BetterString, ID3D11GeometryShader*>::iterator GeometryShadersIter;
	map<BetterString, ID3D11ComputeShader*>::iterator  ComputeShadersIter;

	for (VertexShadersIter = m_VertexShaders.begin();
		 VertexShadersIter != m_VertexShaders.end();
		 VertexShadersIter++)
	{
		SAFE_RELEASE(VertexShadersIter->second);
	}

	for (PixelShadersIter = m_PixelShaders.begin();
		 PixelShadersIter != m_PixelShaders.end();
		 PixelShadersIter++)
	{
		SAFE_RELEASE(PixelShadersIter->second);
	}

	for (GeometryShadersIter = m_GeometryShaders.begin();
		 GeometryShadersIter != m_GeometryShaders.end();
		 GeometryShadersIter++)
	{
		SAFE_RELEASE(GeometryShadersIter->second);
	}

	for (ComputeShadersIter = m_ComputeShaders.begin();
		 ComputeShadersIter != m_ComputeShaders.end();
		 ComputeShadersIter++)
	{
		SAFE_RELEASE(ComputeShadersIter->second);
	}*/
}
//=================================================================================================================
/*void Shader::AddMacro(const std::string& name, UINT32 value)
{
	assert(numDefines < MaxDefines);

    defines[numDefines].Name = buffer + bufferIdx;
    for(UINT32 i = 0; i < name.length(); ++i)
        buffer[bufferIdx++] = name[i];
    ++bufferIdx;

    std::string stringVal = ToAnsiString(value);
    defines[numDefines].Definition = buffer + bufferIdx;
    for(UINT32 i = 0; i < stringVal.length(); ++i)
        buffer[bufferIdx++] = stringVal[i];
    ++bufferIdx;
    
    ++numDefines;
}
//=================================================================================================================
const D3D10_SHADER_MACRO* Shader::Defines() const
{
	return defines;
}*/
//=================================================================================================================
ID3D11VertexShader* Shader::GetVertexShader(BetterString shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_VERTEX);
	return m_pVertexShader->mShader;
}
//=================================================================================================================
ID3D11PixelShader* Shader::GetPixelShader(BetterString shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_PIXEL);
	return m_pPixelShader->mShader;
}
//=================================================================================================================
ID3D11GeometryShader* Shader::GetGeometryShader(BetterString shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_GEOMETRY);
	return m_pGeometryShader->mShader;
}
//=================================================================================================================
ID3D11ComputeShader* Shader::GetComputeShader(BetterString shaderFuncName)
{
	SwitchTo(shaderFuncName, EShaderTypes::ST_COMPUTE);
	return m_pComputeShader->mShader;
}
//=================================================================================================================
void Shader::SetVertexShader()
{
	m_pD3DSystem->GetDeviceContext()->VSSetShader(m_pVertexShader->mShader, NULL, 0);

	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);
}
//=================================================================================================================
void Shader::SetPixelShader()
{
	m_pD3DSystem->GetDeviceContext()->PSSetShader(m_pPixelShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetHullShader()
{
	m_pD3DSystem->GetDeviceContext()->HSSetShader(m_pHullShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetDomainShader()
{
	m_pD3DSystem->GetDeviceContext()->DSSetShader(m_pDomainShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetGeometryShader()
{
	m_pD3DSystem->GetDeviceContext()->GSSetShader(m_pGeometryShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetComputeShader()
{
	m_pD3DSystem->GetDeviceContext()->CSSetShader(m_pComputeShader->mShader, NULL, 0);
}
//=================================================================================================================
void Shader::SetInputLayout(BetterString vertexShaderName)
{
	m_pD3DSystem->GetDeviceContext()->IASetInputLayout(m_VertexShaderLayout[vertexShaderName]->m_layout11);
}
//=================================================================================================================
void Shader::RenderDraw11(int vertexCount)
{
	m_pD3DSystem->GetDeviceContext()->Draw(vertexCount, 0);
}
//=================================================================================================================
void Shader::RenderIndex11(int indexCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}
//=================================================================================================================
void Shader::RenderIndex11(int startIndex, int indexCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexed(indexCount, startIndex, 0);
}
//=================================================================================================================
void Shader::RenderCS11(int threadX, int threadY, int threadZ)
{
	m_pD3DSystem->GetDeviceContext()->Dispatch( threadX, threadY, threadZ );
}
//=================================================================================================================
void Shader::RenderIndexInstanced11(int indexCount, int primCount)
{
	m_pD3DSystem->GetDeviceContext()->DrawIndexedInstanced(indexCount, primCount, 0, 0, 0);
}
//=================================================================================================================
void Shader::SwitchTo(BetterString shaderFuncName, int type)
{
	map<BetterString, VertexShader*>::iterator   VertexShadersIter;
	map<BetterString, PixelShader*>::iterator    PixelShadersIter;
	map<BetterString, GeometryShader*>::iterator GeometryShadersIter;
	map<BetterString, ComputeShader*>::iterator  ComputeShadersIter;

	switch (type)
	{
		case EShaderTypes::ST_VERTEX:
		{
			for (VertexShadersIter = m_VertexShaders.begin();
				 VertexShadersIter != m_VertexShaders.end();
				 VertexShadersIter++)
			{
				if (VertexShadersIter->first == shaderFuncName)
				{
					m_pVertexShader = VertexShadersIter->second;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_PIXEL:
		{
			for (PixelShadersIter = m_PixelShaders.begin();
				 PixelShadersIter != m_PixelShaders.end();
				 PixelShadersIter++)
			{
				if (PixelShadersIter->first == shaderFuncName)
				{
					m_pPixelShader = PixelShadersIter->second;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_GEOMETRY:
		{
			for (GeometryShadersIter = m_GeometryShaders.begin();
				 GeometryShadersIter != m_GeometryShaders.end();
				 GeometryShadersIter++)
			{
				if (GeometryShadersIter->first == shaderFuncName)
				{
					m_pGeometryShader = GeometryShadersIter->second;
					break;
				}
			}
		}
		break;
		case EShaderTypes::ST_COMPUTE:
		{
			for (ComputeShadersIter = m_ComputeShaders.begin();
				 ComputeShadersIter != m_ComputeShaders.end();
				 ComputeShadersIter++)
			{
				if (ComputeShadersIter->first == shaderFuncName)
				{
					m_pComputeShader = ComputeShadersIter->second;
					break;
				}
			}
		}
		break;
	}
}
//=================================================================================================================
ID3D11InputLayout* Shader::GetInputLayout(BetterString vertexShaderName)
{
	if (m_VertexShaderLayout[vertexShaderName] == 0) return 0;
	if (m_VertexShaderLayout[vertexShaderName]->m_layout11 == 0) return 0;
	return m_VertexShaderLayout[vertexShaderName]->m_layout11;
}
//=================================================================================================================
void Shader::SetInputLayoutDesc(BetterString vertexShaderName, D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements)
{
	SLayout* sl = new SLayout();
	
	sl->m_pLayoutDesc = desc;
	sl->m_NumElements = NumElements;
	
	//m_VertexShaderLayout[vertexShaderName] = sl;

	m_VertexShaderLayout.insert(make_pair(vertexShaderName.toString(), sl));
}
//=================================================================================================================
void Shader::ClearInputLayout()
{
	if (m_VertexShaderLayout.size() != 0)
		m_VertexShaderLayout.clear();
}
//=================================================================================================================
void Shader::AssignVertexShaderLayout(BetterString vertexShaderName)
{
	//Create the vertex input layout
	if (m_UseInputLayout)
	{
		ID3DBlob* blob = m_pVertexShader->mBlobData;
		
		map<string, SLayout*>::iterator iter = m_VertexShaderLayout.find(vertexShaderName.toString());

		if (iter != m_VertexShaderLayout.end())
		{
			SLayout* sLayout = (*iter).second;

			ID3D11InputLayout* layout;
			D3D11_INPUT_ELEMENT_DESC* desc = sLayout->m_pLayoutDesc;
			UINT numElements = sLayout->m_NumElements;

			m_pD3DSystem->GetDevice11()->CreateInputLayout(
				desc,
				numElements,
				blob->GetBufferPointer(), blob->GetBufferSize(),
				&layout);

			m_VertexShaderLayout[vertexShaderName]->m_layout11 = layout;

			assert(m_VertexShaderLayout[vertexShaderName]->m_layout11);
		}
	}
}
//=================================================================================================================
void Shader::LoadVertexShader(BetterString shaderFuncName)
{
	m_VertexShaders.insert(make_pair(shaderFuncName, (VertexShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_VERTEX);
}
//=================================================================================================================
void Shader::LoadPixelShader(BetterString shaderFuncName)
{
	m_PixelShaders.insert(make_pair(shaderFuncName, (PixelShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_PIXEL);
}
//=================================================================================================================
void Shader::LoadHullShader(BetterString shaderFuncName)
{
	m_pHullShader = (HullShader*)ShaderCompiler::GetShaderByName(shaderFuncName);
}
//=================================================================================================================
void Shader::LoadDomainShader(BetterString shaderFuncName)
{
	m_pDomainShader = (DomainShader*)ShaderCompiler::GetShaderByName(shaderFuncName);
}
//=================================================================================================================
void Shader::LoadGeometryShader(BetterString shaderFuncName)
{
	m_GeometryShaders.insert(make_pair(shaderFuncName, (GeometryShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_GEOMETRY);
}
//=================================================================================================================
void Shader::LoadComputeShader(BetterString shaderFuncName)
{
	m_ComputeShaders.insert(make_pair(shaderFuncName, (ComputeShader*)ShaderCompiler::GetShaderByName(shaderFuncName)));
	SwitchTo(shaderFuncName, EShaderTypes::ST_COMPUTE);
}
//=================================================================================================================
/*HRESULT Shader::Compile(BetterString filename, BetterString shaderFuncName, ShaderTypes type, bool loadPrecompiledShader)
{
	// Compiling Shaders with D3DCompileFromFile
	//Google: visual studio 2013 hlsl shader compiling
	//http://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	//http://stackoverflow.com/questions/10759300/how-do-i-use-shader-model-5-0-compiled-shaders-in-visual-studio-11
	/*
		Here "PixelShader.cso" is the precompiled hlsl shader generated by Visual Studio 11 from a .hlsl file in the project.
		The compiled .cso file is usually moved to the Projects/ProjectName/Debug folder by default.
		As a result it must be cut and paste into the same directory as your source code before using.
		Mind you this setting can be changed by right-clicking the HLSL file while inside Visual Studio 11 and
		editing the Output Settings. By default its: $(OutDir)%(Filename).cso, change it to: $(Directory)%(Filename).cso
	*
	
	//Google: creating a cso file in DirectX11.1
	//http://stackoverflow.com/questions/5020204/loading-a-precompiled-hlsl-shader-into-memory-for-use-with-createpixelshader
	
	BetterString base = "..\\Framework\\Graphics\\Shaders\\";
	filename = base + filename;

	unique_ptr<wchar_t> name = filename.ToWideStr();

	HRESULT hr;
	ID3DBlob* blob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

	switch (type)
	{
		case ST_VERTEX:
		{
			LPCSTR profile = "vs_5_0";
			//LPCSTR profile = "vs_5_0_level_9_1";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}
			
			hr = m_pD3DSystem->GetDevice11()->CreateVertexShader( blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pVertexShader );
			if( FAILED(hr) )
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}
			
			// Set the new vertex shader
			SetVertexShader();

#if defined(DEBUG) || defined(PROFILE)
			if ( SUCCEEDED(hr) )
			{
				m_pVertexShader->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName );
			}
#endif
			//Add the vertex shader to the list of vertex shaders
			m_VertexShaders.insert( make_pair(shaderFuncName, m_pVertexShader) );

			//Create the vertex input layout
			AssignVertexShaderLayout(shaderFuncName, blob);
		}
		break;
		case ST_HULL:
		{
			LPCSTR profile = "hs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}

			hr = m_pD3DSystem->GetDevice11()->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pHullShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}

			// Set the new hull shader
			SetHullShader();

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				m_pHullShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
		}
		break;
		case ST_DOMAIN:
		{
			LPCSTR profile = "ds_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}

			hr = m_pD3DSystem->GetDevice11()->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pDomainShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}

			// Set the new domain shader
			SetDomainShader();

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				m_pDomainShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
		}
		break;
		case ST_PIXEL:
		{
			LPCSTR profile = "ps_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}

			hr = m_pD3DSystem->GetDevice11()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pPixelShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}

			// Set the new pixel shader
			SetPixelShader();

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				m_pPixelShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			//Add the pixel shader to the list of pixel shaders
			m_PixelShaders.insert(make_pair(shaderFuncName, m_pPixelShader));
		}
		break;
		case ST_GEOMETRY:
		{
			LPCSTR profile = "gs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}

			hr = m_pD3DSystem->GetDevice11()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pGeometryShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}

			// Set the new geometry shader
			SetGeometryShader();

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				m_pGeometryShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			//Add the geometry shader to the list of geometry shaders
			m_GeometryShaders.insert(make_pair(shaderFuncName, m_pGeometryShader));
		}
		break;
		case ST_COMPUTE:
		{
			LPCSTR profile = "cs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, loadPrecompiledShader);
			
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				exit(0);
			}

			hr = m_pD3DSystem->GetDevice11()->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pComputeShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, filename);
				//SAFE_RELEASE( pErrorBlob );
				exit(0);
				//return hr;
			}

			// Set the new compute shader
			SetGeometryShader();

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				m_pComputeShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			//Add the compute shader to the list of compute shaders
			m_ComputeShaders.insert(make_pair(shaderFuncName, m_pComputeShader));
		}
		break;
		default:
		{
			// Nothing
			break;
		}
	}
	
	return S_OK;
}
//=================================================================================================================
ID3DBlob* Shader::Compile(LPCWSTR name, BetterString shaderFuncName, LPCSTR profile, UINT flags, ID3DBlob*& errorBlob, bool loadPrecompiledShader)
{
	HRESULT hr;
	
	ID3DBlob* blob = nullptr;
	
	if (loadPrecompiledShader == false)
	{
		if (numDefines > 0)
		{
			hr = D3DCompileFromFile(name, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				shaderFuncName.toString().c_str(), profile,
				flags, 0, &blob, &errorBlob);
		}
		else
		{
			hr = D3DCompileFromFile(name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				shaderFuncName.toString().c_str(), profile,
				flags, 0, &blob, &errorBlob);
		}

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}

			if (blob)
				blob->Release();
		}
	}
	else
	{
		D3DReadFileToBlob(name, &blob);
	}
	
	return blob;
}
//=================================================================================================================
std::vector<uint8_t> Shader::CompileBinary(const std::string filename)
{
	// Compiles a .cso (Compiled Shader Object) file
	
	std::vector<uint8_t> data;

	BetterString base = "..\\Framework\\Graphics\\Shaders\\";
	BetterString file = base + filename;

	std::ifstream fs;
	fs.open(file.toString(), std::ifstream::in | std::ifstream::binary);
	if (fs.good())
	{
		auto size = CGlobal::FileSize(filename);
		// TODO: check here if size is more than size_t
		data.resize(static_cast<size_t>(size));
		fs.seekg(0, std::ios::beg);
		fs.read(reinterpret_cast<char*>(&data[0]), size);
		fs.close();
	}
	return data;
}
//=================================================================================================================
void Shader::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, string shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(blob->GetBufferPointer());

	bufferSize = blob->GetBufferSize();

	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	blob->Release();
	blob = 0;

	char path[MAX_PATH];
	char buffer[MAX_PATH];
	sprintf_s(buffer, "%s", (char*)shaderFilename.c_str());
	MessageBox(hwnd, "Compile Shader Error", buffer, MB_OK);
}
//=================================================================================================================
_Use_decl_annotations_
HRESULT Shader::FindDXSDKShaderFileCch( CHAR* strDestPath, int cchDest, LPCSTR strFilename )
{
    if( !strFilename || strFilename[0] == 0 || !strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    CHAR strExePath[MAX_PATH] =
    {
        0
    };
    CHAR strExeName[MAX_PATH] =
    {
        0
    };

    CHAR* strLastSlash = nullptr;
    GetModuleFileName( nullptr, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = strchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        strcpy_s( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = strchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    // Search in directories:
    //      .\
    //      %EXE_DIR%\..\..\%EXE_NAME%

    strcpy_s( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    sprintf_s( strDestPath, cchDest, "%s\\..\\..\\%s\\%s", strExePath, strExeName, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;    

    // On failure, return the file as the path but also return an error code
    strcpy_s( strDestPath, cchDest, strFilename );

    return E_FAIL;
}*/
//=================================================================================================================