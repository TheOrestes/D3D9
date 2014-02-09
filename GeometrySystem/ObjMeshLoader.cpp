
#include "ObjMeshLoader.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Scene/Scene.h"
#include "../Game/Camera.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"
#include <fstream>

D3DVERTEXELEMENT9	VERTEX_DECL[] = 
{
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
ObjMeshLoader::ObjMeshLoader()
{
	m_pMesh		=	NULL;
	m_pEffect	=	NULL;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matTranslate);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matScale);
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
ObjMeshLoader::~ObjMeshLoader()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Kill
//------------------------------------------------------------------------
void	ObjMeshLoader::Kill()
{
	std::vector<Material*>::iterator iter = m_Materials.begin();

	for ( ; iter != m_Materials.end() ; ++iter)
	{
		(*iter)->pTexture = NULL;
		SAFE_RELEASE((*iter)->pTexture);
		SAFE_DELETE((*iter));
	}

	m_Materials.clear();
	m_Vertices.clear();
	m_Indices.clear();
	m_Attributes.clear();
	m_strMediaDir.clear();

	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pEffect);
}

//------------------------------------------------------------------------
// Purpose  : Initialize Material
//------------------------------------------------------------------------
void   ObjMeshLoader::InitMaterial(Material& pMaterial)
{
	ZeroMemory(&pMaterial, sizeof(Material));

	pMaterial.bSpecular = false;
	pMaterial.fAlpha = 1.0f;
	pMaterial.nShininess = 0;
	pMaterial.pTexture = NULL;
	pMaterial.vAmbient = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	pMaterial.vDiffuse = D3DXVECTOR3(0.8f, 0.8f, 0.8f);
	pMaterial.vSpecular = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

//------------------------------------------------------------------------
// Purpose  : 
//------------------------------------------------------------------------
bool    ObjMeshLoader::LoadGeometryFromOBJ(const std::string& strFileName)
{
	std::string strMaterialFileName;
	HRESULT Hr;

	// create temp storage for the input data. Once the data has been loaded
	// into a resonable format we can create a D3DXMesh object & load into it mesh data
	std::vector<D3DXVECTOR3> Position;
	std::vector<D3DXVECTOR2> TexCoords;
	std::vector<D3DXVECTOR3> Normals;

	Material* pMaterial = new Material();
	if (!pMaterial)
	    return false;

	// First subset uses default material
	InitMaterial(*pMaterial);
	pMaterial->strName.assign("default");
	m_Materials.push_back(pMaterial);
	
	DWORD dwCurSubset = 0;

	// set the media directory
	m_strMediaDir.assign(Helper::ExtractDirectory(strFileName));

	// File Input
	char objTag[256] = {0};
	std::ifstream inFile(strFileName.c_str());
	if(!inFile)
	{
		LogManager::GetInstance()->WriteToFile(strFileName.c_str(), " loading FAILED!");
		return false;
	}

	for (; ;)
	{
		inFile>>objTag;
		if(!inFile)
			break;

		if(!strcmp(objTag, "#"))
		{
			// comment
		}
		else if (!strcmp(objTag, "v"))
		{
			// vertex position
			float x,y,z;
			inFile>>x >>y >>z;
			Position.push_back(D3DXVECTOR3(x,y,z));
		}
		else if (!strcmp(objTag, "vt"))
		{
			// texture coord
			float u,v;
			inFile>>u >>v;
			TexCoords.push_back(D3DXVECTOR2(u,v));
		}
		else if(!strcmp(objTag, "vn"))
		{
			// vertex normal
			float x,y,z;
			inFile>>x >>y >>z;
			Normals.push_back(D3DXVECTOR3(x,y,z));
		}
		else if(!strcmp(objTag, "mtllib"))
		{
			// material library
			inFile>>strMaterialFileName;
		}
		else if(!strcmp(objTag, "usemtl"))
		{
			// Material
			std::string strName;
			std::getline(inFile, strName);

			bool bFound = false;
			for (int i = 0 ; i<m_Materials.size() ; i++)
			{
				Material* pCurMaterial = m_Materials.at(i);
				if(!strcmp(pCurMaterial->strName.c_str(), strName.c_str()))
				{
					bFound = true;
					dwCurSubset = i;
					break;
				}
			}

			if(!bFound)
			{
				pMaterial = new Material();
				dwCurSubset = m_Materials.size();

				InitMaterial(*pMaterial);
				pMaterial->strName.assign(strName.c_str());

				m_Materials.push_back(pMaterial);
			}
		}
		else if(!strcmp(objTag, "f"))
		{
			// Face data
			UINT iPosition, iTexCoord, iNormal;
			VERTEX vertex;

			for (UINT iFace = 0 ; iFace<3 ; iFace++)
			{
				ZeroMemory(&vertex, sizeof(VERTEX));

				// OBJ format uses 1-based arrays
				inFile>>iPosition;
				vertex.position = Position[iPosition-1];
				
				if('/' == inFile.peek())
				{
					inFile.ignore();

					if('/' != inFile.peek())
					{
						// optional texture co-ordinates
						inFile>>iTexCoord;
						vertex.texcoord = TexCoords[iTexCoord-1];
					}

					if('/' == inFile.peek())
					{
						inFile.ignore();

						// optional normal info
						inFile>>iNormal;
						vertex.normal = Normals[iNormal-1];
					}
				}

				DWORD index = AddVertex(iPosition, vertex);
				m_Indices.push_back(index);
			}
			m_Attributes.push_back(dwCurSubset);
		}
		else
		{
			// unrecognized command
		}

		inFile.ignore(1000, '\n');
	}
	
	//cleanup
	inFile.close();
	DeleteCache();

	// If an associated material file was found, read that as well
	if (strMaterialFileName.c_str())
	{
		LoadMaterialsFromMTL(strMaterialFileName);
	}

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Add Vertex
//------------------------------------------------------------------------
DWORD	ObjMeshLoader::AddVertex( UINT hash, VERTEX& pVertex )
{
	// If this vertex doesn't already exist in the Vertices list, create
	// new entry. Add the index of the vertex to the indices list.
	bool bFoundInList = false;
	DWORD index = 0;

	// Since its very slow to check every element in the vertex list,
	// a hash-table stores vertex indices according to the vertex position's
	// index as reported by the OBJ file.
	if ((UINT)m_VertexCache.size() > hash)
	{
		CacheEntry* pEntry = m_VertexCache.at(hash);
		while (pEntry)
		{
			VERTEX*	pCacheVertex = m_Vertices.data() + pEntry->index;

			// If this vertex is identical to the vertex already in the list, 
			// simply point the index buffer to the existing vertex
			if(0 == memcmp(&pVertex, pCacheVertex, sizeof(VERTEX)))
			{
				bFoundInList = true;
				index = pEntry->index;
				break;
			}

			pEntry = pEntry->pNext;
		}
	}

	// Vertex was not found in the list. Create a new entry, both within
	// the Vertices list & also within hash-table cache.
	if (!bFoundInList)
	{
		// Add to the vertices list
		index = m_Vertices.size();
		m_Vertices.push_back(pVertex);

		// Add this to the hash-table
		CacheEntry* pNewEntry = new CacheEntry();
		pNewEntry->index = index;
		pNewEntry->pNext = NULL;

		// grow the hash table if required
		while ((UINT)m_VertexCache.size() <= hash)
		{
			m_VertexCache.push_back(NULL);
		}

		// Add to the end of the linked list
		CacheEntry* pCurEntry = m_VertexCache.at( hash );
		if( pCurEntry == NULL )
		{
			m_VertexCache.push_back(pNewEntry);
		}
		else
		{
			// Find the tail
			while( pCurEntry->pNext != NULL )
			{
				pCurEntry = pCurEntry->pNext;
			}

			pCurEntry->pNext = pNewEntry;
		}
	}

	return index;
}

//------------------------------------------------------------------------
// Purpose  : Load materials from MTL file
//------------------------------------------------------------------------
HRESULT		ObjMeshLoader::LoadMaterialsFromMTL( const std::string& strFileName )
{
	HRESULT hr;

	// File input
	char mtrlTag[256];
	// Get absolute path
	std::string mediaPath = m_strMediaDir;
	mediaPath.append(strFileName);
	std::ifstream InFile( mediaPath.c_str() );
	if( !InFile )
	{
		LogManager::GetInstance()->WriteToFile(strFileName.c_str(), " loading FAILED!");
		return S_FALSE;
	}

	Material* pMaterial = NULL;

	for(; ; )
	{
		InFile >> mtrlTag;
		if( !InFile )
			break;

		if( !strcmp( mtrlTag, "newmtl" ) )
		{
			std::string strName;
			std::getline(InFile, strName);

			pMaterial = NULL;
			for( int i = 0; i < m_Materials.size(); i++ )
			{
				Material* pCurMaterial = m_Materials.at( i );
				if( !strcmp( pCurMaterial->strName.c_str(), strName.c_str()) )
				{
					pMaterial = pCurMaterial;
					break;
				}
			}
		}

		// The rest of the commands rely on an active material
		if( pMaterial == NULL )
			continue;

		if( !strcmp( mtrlTag, "#" ) )
		{
			// Comment
		}
		else if( !strcmp( mtrlTag, "Ka" ) )
		{
			// Ambient color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->vAmbient = D3DXVECTOR3( r, g, b );
		}
		else if( !strcmp( mtrlTag, "Kd" ) )
		{
			// Diffuse color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->vDiffuse = D3DXVECTOR3( r, g, b );
		}
		else if( !strcmp( mtrlTag, "Ks" ) )
		{
			// Specular color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->vSpecular = D3DXVECTOR3( r, g, b );
		}
		else if( !strcmp( mtrlTag, "d" ) || !strcmp( mtrlTag, "Tr" ) )
		{
			// Alpha
			InFile >> pMaterial->fAlpha;
		}
		else if( !strcmp( mtrlTag, "Ns" ) )
		{
			// Shininess
			int nShininess;
			InFile >> nShininess;
			pMaterial->nShininess = nShininess;
		}
		else if( !strcmp( mtrlTag, "illum" ) )
		{
			// Specular on/off
			int illumination;
			InFile >> illumination;
			pMaterial->bSpecular = ( illumination == 2 );
		}
		else if( !strcmp( mtrlTag, "map_Kd" ) )
		{
			// Texture
			InFile >> pMaterial->strTexture;
		}

		else
		{
			// Unimplemented or unrecognized command
		}

		InFile.ignore( 1000, L'\n' );
	}

	InFile.close();

	return S_OK;
}

//------------------------------------------------------------------------
// Purpose  : Create
//------------------------------------------------------------------------
bool    ObjMeshLoader::Init(const std::string& strFileName)
{
	HRESULT Hr;
	LPDIRECT3DDEVICE9 device = RenderDevice::GetInstance()->getDevice();

	// Load the vertex buffer, index buffer, and subset information from a file. 
	// In this case, an .obj file was chosen for simplicity, but it's meant to illustrate 
	// that ID3DXMesh objects can be filled from any mesh file format once the necessary 
	// data is extracted from file.
	if(!LoadGeometryFromOBJ(strFileName))
	{
		LogManager::GetInstance()->WriteToFile(strFileName.c_str(), "Load FAILED!");
		return false;
	}

	// Load Material textures
	for (int i = 0 ; i<m_Materials.size() ; i++)
	{
		Material*	pMaterial = m_Materials.at(i);
		if(!pMaterial->strTexture.empty())
		{
			// avoid loading same texture twice
			bool bFound = false;
			for (int x = 0 ; x<i ; x++)
			{
				Material* pCur = m_Materials.at(x);
				if(!pCur->strTexture.compare(pMaterial->strTexture))
				{
					bFound = true;
					pMaterial->pTexture = pCur->pTexture;
					break;
				}
			}

			// Not found, load the texture!
			if(!bFound)
			{
				// Get absolute path
				std::string mediaPath = m_strMediaDir;
				mediaPath.append(pMaterial->strTexture);
				Hr = D3DXCreateTextureFromFile(	device, 
												mediaPath.c_str(),
												&(pMaterial->pTexture));

				if(S_OK != Hr)
				{
					LogManager::GetInstance()->WriteToFile("D3DXCreateTextureFromFile() FAILED!");
					return false;
				}
			}
		}
	}

	// Create the encapsulated mesh
	LPD3DXMESH	pMesh = NULL;
	Hr = D3DXCreateMesh(m_Indices.size()/3, m_Vertices.size(), 
								D3DXMESH_MANAGED|D3DXMESH_32BIT, VERTEX_DECL,
								device, &pMesh);

	if (S_OK != Hr)
	{
		LogManager::GetInstance()->WriteToFile("D3DXCreateMesh() FAILED!");
		return false;
	}

	// Copy Vertex Data
	VERTEX* pVertex;
	pMesh->LockVertexBuffer(0, (void**)&pVertex);
	memcpy(pVertex, m_Vertices.data(), m_Vertices.size()*sizeof(VERTEX));
	pMesh->UnlockVertexBuffer();
	m_Vertices.clear();

	// Copy Index Data
	DWORD*	pIndex;
	pMesh->LockIndexBuffer(0, (void**)&pIndex);
	memcpy(pIndex, m_Indices.data(), m_Indices.size()*sizeof(DWORD));
	pMesh->UnlockIndexBuffer();
	m_Indices.clear();

	// Copy Attribute Data
	DWORD*	pSubset;
	pMesh->LockAttributeBuffer(0, &pSubset);
	memcpy(pSubset, m_Attributes.data(), m_Attributes.size()*sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();
	m_Attributes.clear();

	// Reorder the vertices according to subset and optimize the mesh for this graphics 
	// card's vertex cache. When rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader.
	DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if( aAdjacency == NULL )
		return E_OUTOFMEMORY;

	pMesh->GenerateAdjacency( 1e-6f, aAdjacency );
	pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, 
							NULL, NULL, NULL );

	SAFE_DELETE_ARRAY( aAdjacency );
	m_pMesh = pMesh;

	// Create Effect related to the mesh
	Hr = D3DXCreateEffectFromFile(	device, "Shaders\\FX\\MeshFromOBJ.fx", NULL, 
									NULL, NULL, NULL, &m_pEffect, NULL );

	if (S_OK != Hr)
	{
		LogManager::GetInstance()->WriteToFile("D3DXCreateEffectFromFile() FAILED!");
		return false;
	}
	else
	{
		// Cache the effect handles
		g_hAmbient = m_pEffect->GetParameterBySemantic( 0, "Ambient" );
		g_hDiffuse = m_pEffect->GetParameterBySemantic( 0, "Diffuse" );
		g_hSpecular = m_pEffect->GetParameterBySemantic( 0, "Specular" );
		g_hOpacity = m_pEffect->GetParameterBySemantic( 0, "Opacity" );
		g_hSpecularPower = m_pEffect->GetParameterBySemantic( 0, "SpecularPower" );
		g_hLightColor = m_pEffect->GetParameterBySemantic( 0, "LightColor" );
		g_hLightPosition = m_pEffect->GetParameterBySemantic( 0, "LightPosition" );
		g_hCameraPosition = m_pEffect->GetParameterBySemantic( 0, "CameraPosition" );
		g_hTexture = m_pEffect->GetParameterBySemantic( 0, "Texture" );
		g_hTime = m_pEffect->GetParameterBySemantic( 0, "Time" );
		g_hWorld = m_pEffect->GetParameterBySemantic( 0, "World" );
		g_hWorldViewProjection = m_pEffect->GetParameterBySemantic( 0, "WorldViewProjection" );

		// Store the correct technique handles for each material
		for( UINT i = 0; i < m_Materials.size(); i++ )
		{
			Material* pMaterial = GetMaterial( i );

			const char* strTechnique = NULL;

			if( pMaterial->pTexture && pMaterial->bSpecular )
				strTechnique = "TexturedSpecular";
			else if( pMaterial->pTexture && !pMaterial->bSpecular )
				strTechnique = "TexturedNoSpecular";
			else if( !pMaterial->pTexture && pMaterial->bSpecular )
				strTechnique = "Specular";
			else if( !pMaterial->pTexture && !pMaterial->bSpecular )
				strTechnique = "NoSpecular";

			pMaterial->hTechnique = m_pEffect->GetTechniqueByName( strTechnique );
		}
	}

	return true;
}

//------------------------------------------------------------------------
// Purpose  : Render Subset
//------------------------------------------------------------------------
void ObjMeshLoader::RenderSubset( UINT index )
{
	HRESULT hr;
	UINT iPass, cPasses;

	// Retrieve the ID3DXMesh pointer and current material from the MeshLoader helper
	ID3DXMesh* pMesh = GetMesh();
	Material* pMaterial = GetMaterial( index );

	// Set the lighting variables and texture for the current material
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

	D3DXVECTOR3 tempEye = Camera::GetInstance()->m_Position;
	D3DXVECTOR4	Eye(tempEye.x, tempEye.y, tempEye.z, 1.0f);

	if (m_pEffect)
	{
		m_pEffect->SetMatrix( g_hWorldViewProjection, &wvp );
		m_pEffect->SetMatrix( g_hWorld, &matWorld );

		m_pEffect->SetValue( g_hCameraPosition, &Eye, sizeof( D3DXVECTOR3 ) );

		m_pEffect->SetValue( g_hAmbient, pMaterial->vAmbient, sizeof( D3DXVECTOR3 ) );
		m_pEffect->SetValue( g_hDiffuse, pMaterial->vDiffuse, sizeof( D3DXVECTOR3 ) );
		m_pEffect->SetValue( g_hSpecular, pMaterial->vSpecular, sizeof( D3DXVECTOR3 ) );
		m_pEffect->SetTexture( g_hTexture, pMaterial->pTexture );
		m_pEffect->SetFloat( g_hOpacity, pMaterial->fAlpha );
		m_pEffect->SetInt( g_hSpecularPower, pMaterial->nShininess );

		m_pEffect->SetTechnique( pMaterial->hTechnique );
		m_pEffect->Begin( &cPasses, 0 );

		for( iPass = 0; iPass < cPasses; iPass++ )
		{
			m_pEffect->BeginPass( iPass );

			// The effect interface queues up the changes and performs them 
			// with the CommitChanges call. You do not need to call CommitChanges if 
			// you are not setting any parameters between the BeginPass and EndPass.
			// V( m_pEffect->CommitChanges() );

			// Render the mesh with the applied technique
			pMesh->DrawSubset( index );

			m_pEffect->EndPass();
		}
		m_pEffect->End();
	}
}

//------------------------------------------------------------------------
// Purpose  : Translate
//------------------------------------------------------------------------
void	ObjMeshLoader::Translate(const D3DXVECTOR3& axis)
{
	D3DXMatrixTranslation(&matTranslate, axis.x, axis.y, axis.z);
}

//------------------------------------------------------------------------
// Purpose  : Rotate
//------------------------------------------------------------------------
void	ObjMeshLoader::Rotate(const D3DXVECTOR3& axis, float angle)
{
	D3DXMatrixRotationAxis(&matRotation, &axis, angle);
}

//------------------------------------------------------------------------
// Purpose  : Scale
//------------------------------------------------------------------------
void	ObjMeshLoader::Scale(const D3DXVECTOR3& axis)
{
	D3DXMatrixScaling(&matScale, axis.x, axis.y, axis.z);
}

//------------------------------------------------------------------------
// Purpose  : Update
//------------------------------------------------------------------------
void ObjMeshLoader::Update( float dt )
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat( g_hTime, dt );
	}
	
	matWorld = matScale * matRotation * matTranslate;
}

//------------------------------------------------------------------------
// Purpose  : Delete Cache
//------------------------------------------------------------------------
void	ObjMeshLoader::DeleteCache()
{
	// Iterate through all the elements in the cache & subsequent linked lists
	for (int i = 0 ; i<m_VertexCache.size() ; i++)
	{
		CacheEntry*	pEntry = m_VertexCache.at(i);
		while (pEntry != NULL)
		{
			CacheEntry* pNext = pEntry->pNext;
			SAFE_DELETE(pEntry);
			pEntry = pNext;
		}
	}

	m_VertexCache.clear();
}


































