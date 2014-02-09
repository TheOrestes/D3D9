#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"
#include "../ShaderEngine/FxShader.h"
#include "../Misc/EngineStats.h"
#include "../Scene/Scene.h"
#include "../Game/Camera.h"
#include "../Renderables/Skybox.h"
#include "../GeometrySystem/Vertex.h"
#include "StaticGeometry.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
StaticGeometry::StaticGeometry()
{
	pGeomFileName		=	NULL;
	pAdjacency			=	NULL;
	pMaterials			=	NULL;
	pEffectInstances	=	NULL;	
	dwNumMaterials		=	NULL;
	pMesh				=	NULL;

	// TRANSFORMATION
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTranslate);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matScale);
	
	tex.clear();
	mtrls.clear();

	// Default Textures
	mDefaultTex			=	NULL;
	mEnvmapTex			=	NULL;

	// SHADER
	pFxShader			=	NULL;

	// Static Geometry Extra Data
	m_statGeomData		=	NULL;
}

//------------------------------------------------------------------------ 
// Purpose  : Destructor
//------------------------------------------------------------------------
StaticGeometry::~StaticGeometry()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Initialize Geometry
//------------------------------------------------------------------------
bool	StaticGeometry::Initialize(const std::string& filename)
{
// 	bool retVal;
// 	LPD3DXMESH	tempMesh;
// 
// 	IDirect3DDevice9*	device = RenderDevice::GetInstance()->getDevice();
// 	LogManager*	LOG	= LogManager::GetInstance();
// 	
// 	// Load Mesh from file
// 	HRESULT	Hr = D3DXLoadMeshFromX(	filename, 
// 									D3DXMESH_MANAGED, 
// 									device, 
// 									&pAdjacency, 
// 									&pMaterials, 
// 									&pEffectInstances, 
// 									&dwNumMaterials, 
// 									&tempMesh );
// 
// 	// Loading Problem?
// 	if (Hr != S_OK)
// 	{
// 		retVal = false;
// 		LOG->WriteToFile(filename, "load failed!");
// 	}
// 	else
// 	{
// 		retVal = true;
// 		LOG->WriteToFile(filename, "loaded successfully!");
// 	}
// 
// 	// Update Engine Stats
// 	EngineStats::GetInstance()->addVertices(tempMesh->GetNumVertices());
// 	EngineStats::GetInstance()->addTriangles(tempMesh->GetNumFaces());
// 
// 	// Load the default texture for the mesh
// 	LPCSTR defTexPath = "Data\\StaticGeometry\\Default.png";
// 	if (S_OK != D3DXCreateTextureFromFile(device, defTexPath, &mDefaultTex))
// 	{
// 		LOG->WriteToFile(defTexPath, "associated texture create failed");
// 		return false;
// 	}
// 
// 	//  [10/25/2010 DoD]
// 	//	Check if loaded mesh already has the NORMAL info
// 	D3DVERTEXELEMENT9 elem[MAX_FVF_DECL_SIZE];
// 	tempMesh->GetDeclaration(elem);
// 
// 	bool bHasNormals = false;
// 	D3DVERTEXELEMENT9 term = D3DDECL_END();
// 	for (int i = 0 ; i<MAX_FVF_DECL_SIZE ; ++i)
// 	{
// 		// search till D3DDECL_END
// 		if (elem[i].Stream == 0xff)
// 			break;
// 
// 		if (elem[i].Type == D3DDECLTYPE_FLOAT3 &&
// 			elem[i].Usage == D3DDECLUSAGE_NORMAL &&
// 			elem[i].UsageIndex == 0)
// 		{
// 			bHasNormals = true;
// 			break;
// 		}
// 	}
// 
// 	// Change Vertex format to VertexPNT
// 	D3DVERTEXELEMENT9	elements[64];
// 	UINT numElements = 0;
// 	VertexPNT::pVertDecl->GetDeclaration(elements, &numElements);
// 
// 	LPD3DXMESH	temp = NULL;
// 	if(S_OK != tempMesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, device, &temp))
// 	{
// 		LOG->WriteToFile(filename, " Mesh Clone Failed");
// 		return false;
// 	}
// 
// 	// done with temp mesh
// 	tempMesh->Release();
// 	tempMesh = NULL;
// 	tempMesh = temp;
// 
// 	// If mesh did not have normals, generate!
// 	if (!bHasNormals)
// 	{
// 		if (S_OK != D3DXComputeNormals(tempMesh, 0))
// 		{
// 			LOG->WriteToFile(filename, " Computing Normal Failed");
// 			return false;
// 		}
// 	}
// 
// 	// Optimize the Mesh
// 	if (S_OK != tempMesh->Optimize(D3DXMESH_MANAGED|D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE,
// 		(DWORD*)pAdjacency->GetBufferPointer(), 0, 0, 0, &pMesh))
// 	{
// 		LOG->WriteToFile(filename, " Mesh Optimize Failed");
// 		return false;
// 	}
// 	
// 	// Done with tempMesh
// 	tempMesh->Release();
// 	tempMesh = NULL;
// 
// 	// Done with AdjBuffer
// 	pAdjacency->Release();
// 	pAdjacency = NULL;
// 
// 	// Extract the materials & load the textures
// 
// 	if (pMaterials != 0 && dwNumMaterials != 0)
// 	{
// 		LPD3DXMATERIAL	d3dxMtrls = (LPD3DXMATERIAL)pMaterials->GetBufferPointer();
// 
// 		for (DWORD i = 0 ; i < dwNumMaterials ; ++i)
// 		{
// 			// Save the ith Material
// 			D3DHelper::Mtrl	m;
// 
// 			m.ambient	=	d3dxMtrls[i].MatD3D.Diffuse;
// 			m.diffuse	=	d3dxMtrls[i].MatD3D.Diffuse;
// 			m.spec		=	d3dxMtrls[i].MatD3D.Specular;
// 			m.specPower	=	d3dxMtrls[i].MatD3D.Power;
// 			mtrls.push_back( m );
// 
// 			// Check if this material has texture
// 			if (d3dxMtrls[i].pTextureFilename != 0)
// 			{
// 				// Yes, so load it
// 				LPDIRECT3DTEXTURE9	tempTex = 0;
// 				LPCSTR texName = d3dxMtrls[i].pTextureFilename;
// 
// 				//////////////////////////////////////////////////////////////////////////
// 				// Do padding here i.e. Texture path + texName
// 				// Basically, D3DXCreateTextureFromFile() expects complete path as X file 
// 				// refers only to texture name not the path, hence we do the padding, so
// 				// that system gets the texture its looking for ^_^
// 				const std::string file = filename;
// 				std::string filePath = Helper::ExtractDirectory(file);
// 				filePath.append(texName);
// 				//////////////////////////////////////////////////////////////////////////
// 
// 				if (S_OK != D3DXCreateTextureFromFile(device, filePath.c_str(), &tempTex))
// 				{
// 					LOG->WriteToFile(filename, "associated texture create failed");
// 					return false;
// 				}
// 
// 				// Save loaded texture
// 				tex.push_back(tempTex);
// 			}
// 			else
// 			{
// 				// NO texture
// 				// Push default texture
// 				tex.push_back(mDefaultTex);
// 			}
// 		}
// 	}
// 
// 	// done with buffer
// 	pMaterials->Release();
// 	pMaterials = NULL;
// 
// 	// Shader
// 	//InitEffect();
// 	pFxShader = new FxShader();
// 	pFxShader->CreateShader(1);
// 
// 	return retVal;
	return true;
}

//------------------------------------------------------------------------
// Purpose  : 
//------------------------------------------------------------------------
bool	StaticGeometry::Initialize(const Helper::StaticGeomData& data)
{
	bool retVal;
	LPD3DXMESH	tempMesh;

	IDirect3DDevice9*	device = RenderDevice::GetInstance()->getDevice();
	LogManager*	LOG	= LogManager::GetInstance();

	// Load Mesh from file
	HRESULT	Hr = D3DXLoadMeshFromX(	data.path.c_str(), 
									D3DXMESH_MANAGED, 
									device, 
									&pAdjacency, 
									&pMaterials, 
									&pEffectInstances, 
									&dwNumMaterials, 
									&tempMesh );

	// Loading Problem?
	if (Hr != S_OK)
	{
		LOG->WriteToFile(data.path.c_str(), "load failed!");
		return false;
	}
	else
	{
		retVal = true;
		LOG->WriteToFile(data.path.c_str(), "loaded successfully!");
	}

	// Update Engine Stats
	EngineStats::GetInstance()->addVertices(tempMesh->GetNumVertices());
	EngineStats::GetInstance()->addTriangles(tempMesh->GetNumFaces());

	// Load the default texture for the mesh
	LPCSTR defTexPath = "Data\\StaticGeometry\\Default.png";
	if (S_OK != D3DXCreateTextureFromFile(device, defTexPath, &mDefaultTex))
	{
		LOG->WriteToFile(defTexPath, "associated texture create failed");
		return false;
	}

	// Load the default environment map texture
	// We can grab this from SkyMap texture, as that is the envmap
	// we should be using logically!
	mEnvmapTex = Scene::GetInstance()->getSkyBoxObject()->getEnvMap();

	//	Check if loaded mesh already has the NORMAL info
	D3DVERTEXELEMENT9 elem[MAX_FVF_DECL_SIZE];
	tempMesh->GetDeclaration(elem);

	bool bHasNormals = false;
	D3DVERTEXELEMENT9 term = D3DDECL_END();
	for (int i = 0 ; i<MAX_FVF_DECL_SIZE ; ++i)
	{
		// search till D3DDECL_END
		if (elem[i].Stream == 0xff)
			break;

		if (elem[i].Type == D3DDECLTYPE_FLOAT3 &&
			elem[i].Usage == D3DDECLUSAGE_NORMAL &&
			elem[i].UsageIndex == 0)
		{
			bHasNormals = true;
			break;
		}
	}

	// Change Vertex format to VertexPNT
	D3DVERTEXELEMENT9	elements[64];
	UINT numElements = 0;
	VertexPNTZ::pVertDecl->GetDeclaration(elements, &numElements);

	LPD3DXMESH	temp = NULL;
	if(S_OK != tempMesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, device, &temp))
	{
		LOG->WriteToFile(data.path.c_str(), " Mesh Clone Failed");
		return false;
	}

	// done with temp mesh
	tempMesh->Release();
	tempMesh = NULL;
	tempMesh = temp;

	// If mesh did not have normals, generate!
	if (!bHasNormals)
	{
		if (S_OK != D3DXComputeNormals(tempMesh, 0))
		{
			LOG->WriteToFile(data.path.c_str(), " Computing Normal Failed");
			return false;
		}
	}

	// Optimize the Mesh
	if (S_OK != tempMesh->Optimize(D3DXMESH_MANAGED|D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_VERTEXCACHE,
		(DWORD*)pAdjacency->GetBufferPointer(), 0, 0, 0, &pMesh))
	{
		LOG->WriteToFile(data.path.c_str(), " Mesh Optimize Failed");
		return false;
	}

	// Done with tempMesh
	tempMesh->Release();
	tempMesh = NULL;

	// Done with AdjBuffer
	pAdjacency->Release();
	pAdjacency = NULL;

	// Shader
	//InitEffect();
	pFxShader = new FxShader();
	pFxShader->CreateShader(data.shader);

	// Extract the materials & load the textures

	if (pMaterials != 0 && dwNumMaterials != 0)
	{
		LPD3DXMATERIAL	d3dxMtrls = (LPD3DXMATERIAL)pMaterials->GetBufferPointer();

		for (DWORD i = 0 ; i < dwNumMaterials ; ++i)
		{
			// Save the ith Material
			D3DHelper::Mtrl	m;

			m.ambient	=	d3dxMtrls[i].MatD3D.Diffuse;
			m.diffuse	=	d3dxMtrls[i].MatD3D.Diffuse;
			m.spec		=	d3dxMtrls[i].MatD3D.Specular;
			m.specPower	=	d3dxMtrls[i].MatD3D.Power;
			mtrls.push_back( m );

			// Check if this material has texture
			if (d3dxMtrls[i].pTextureFilename != 0)
			{
				// Yes, so load it
				LPDIRECT3DTEXTURE9	tempTex = 0;
				std::string texName = d3dxMtrls[i].pTextureFilename;

				//////////////////////////////////////////////////////////////////////////
				// Do padding here i.e. Texture path + texName
				// Basically, D3DXCreateTextureFromFile() expects complete path as X file 
				// refers only to texture name not the path, hence we do the padding, so
				// that system gets the texture its looking for ^_^
				const std::string file = data.path;
				std::string dirPath = Helper::ExtractDirectory(file);

				std::string filePath = dirPath;
				filePath.append(texName);
				//////////////////////////////////////////////////////////////////////////

				if (S_OK != D3DXCreateTextureFromFile(device, filePath.c_str(), &tempTex))
				{
					LOG->WriteToFile(data.path.c_str(), "associated texture create failed");
					return false;
				}

				// Save loaded texture
				tex.push_back(tempTex);
			}
			else
			{
				// NO texture
				// Push default texture
				tex.push_back(mDefaultTex);
			}
		}
	}

	// done with buffer
	pMaterials->Release();
	pMaterials = NULL;

	// Assign input data to Extra Data of THIS static Geometry
	m_statGeomData = new Helper::StaticGeomData(data.path, data.shader);

	return true;
}

//------------------------------------------------------------------------  
// Purpose  : Setup various render states
//------------------------------------------------------------------------
void	StaticGeometry::preRender()
{
	//IDirect3DDevice9*	device = RenderDevice::GetInstance()->getDevice();
	//device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

//------------------------------------------------------------------------
// Purpose  : Render Geometry
//------------------------------------------------------------------------
void	StaticGeometry::Render()
{ 	
	pFxShader->SetShaderTechnique(m_statGeomData->shader);
	

	LPD3DXEFFECT pEffect = pFxShader->GetEffectPtr();

	// The 0 specifies that ID3DXEffect::Begin and ID3DXEffect::End will 
	// save and restore all state modified by the effect.
	UINT uPasses;
	pEffect->Begin( &uPasses, 0 );

	for( UINT uPass = 0; uPass < uPasses; ++uPass )
	{
		pEffect->BeginPass( uPass );

		for (int j = 0 ; j < mtrls.size() ; ++j)
		{
			// if there is a texture, use it
			if (tex[j] != 0)
			{
				// Check this object has environment mapping on it!
				if (m_statGeomData->shader.compare("EnvMap")== 0)
				{
					pFxShader->SetTextureVariable("envTexture", mEnvmapTex);
				}

				pFxShader->SetTextureVariable("baseTexture", tex[j]);
			}
			else
			{
				pFxShader->SetTextureVariable("baseTexture", mDefaultTex);
			}

			pEffect->CommitChanges();
			pMesh->DrawSubset(j);
		}

		pEffect->EndPass();
	}

	pEffect->End();
}

//------------------------------------------------------------------------ 
// Purpose  : for PostProcessing/Changing render states
//------------------------------------------------------------------------
void	StaticGeometry::PostRender()
{	
	//IDirect3DDevice9*	device = RenderDevice::GetInstance()->getDevice();
	//device->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//------------------------------------------------------------------------
// Purpose  : Translate
//------------------------------------------------------------------------
void	StaticGeometry::Translate(const D3DXVECTOR3& axis)
{
	D3DXMatrixTranslation(&matTranslate, axis.x, axis.y, axis.z);
}

//------------------------------------------------------------------------ 
// Purpose  : Rotate
//------------------------------------------------------------------------
void	StaticGeometry::Rotate(const D3DXVECTOR3& axis, float angle)
{
	D3DXMatrixRotationAxis(&matRotation, &axis, angle);
}

//------------------------------------------------------------------------ 
// Purpose  : Scale
//------------------------------------------------------------------------
void	StaticGeometry::Scale(const D3DXVECTOR3& axis)
{
	D3DXMatrixScaling(&matScale, axis.x, axis.y, axis.z);
}

//------------------------------------------------------------------------
// Purpose  : Update
//------------------------------------------------------------------------
void	StaticGeometry::Update(float dt)
{
	matWorld = matScale * matRotation * matTranslate;	
}

//------------------------------------------------------------------------ 
// Purpose  : Set Shader Variables
//------------------------------------------------------------------------
void	StaticGeometry::SetShaderVariables()
{
	// Get View & Projection Matrices from the Shader Base class
	RenderDevice* device = RenderDevice::GetInstance();
	
	D3DXMATRIX* matView = device->GetGlobalViewMatrix();
	D3DXMATRIX* matProj = device->GetGlobalProjectionMatrix();

	// Create WorldViewProjection Matrix for Shader
	D3DXMATRIX matViewVal = *matView;
	D3DXMATRIX matProjVal = *matProj;

	D3DXMATRIX wvp;
	
	wvp = matWorld * matViewVal * matProjVal;

	D3DXVECTOR4 LightDirection = Scene::GetInstance()->m_GlobalLightDirection;
	D3DXVECTOR4 SpecularColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 tempEye = Camera::GetInstance()->m_Position;
	D3DXVECTOR4	Eye(tempEye.x, tempEye.y, tempEye.z, 1.0f);

	// Set WorldViewProjection Matrix for Shader
	//pEffect->SetMatrix("matWorldViewProjection", &wvp);
	pFxShader->SetMatrixShaderVariable("matWorld", matWorld);
	pFxShader->SetMatrixShaderVariable("matWorldViewProjection", wvp);
	pFxShader->SetVector4ShaderVariable("lightDir", LightDirection);
	pFxShader->SetVector4ShaderVariable("SpecCol", SpecularColor);
	pFxShader->SetVector4ShaderVariable("Eye", Eye);
}

//------------------------------------------------------------------------ 
// Purpose  : Kill Geometry
//------------------------------------------------------------------------
void	StaticGeometry::Kill()
{
	SAFE_RELEASE(mDefaultTex);
	SAFE_RELEASE(pMesh);

	if( pFxShader)
	{
		pFxShader->Kill();
	}

	pEffectInstances = NULL;
	pMaterials = NULL;

	pGeomFileName		=	NULL;
	m_statGeomData		=	NULL;
	
	tex.clear();
	mtrls.clear();

	LogManager::GetInstance()->WriteToFile("StaticGeometry Shutdown...");
}

//------------------------------------------------------------------------
// Purpose  : Get World Space position of this mesh
//------------------------------------------------------------------------
D3DXVECTOR4* StaticGeometry::GetWorldPosition()
{
	D3DXVECTOR4 pos(matWorld._41, matWorld._42, matWorld._43, matWorld._44);
	return &pos;
}
