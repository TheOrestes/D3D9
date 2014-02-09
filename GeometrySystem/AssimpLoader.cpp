
#include "AssimpLoader.h"
#include "Model.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"

//////////////////////////////////////////////////////////////////////////
AssimpLoader::AssimpLoader()
{

	// what I am trying to do here is, after assimp successfully loaded & filled up
	// VB & IB for all aiMesh objects with all aiMaterial information, we should pass
	// on all the information to Model class object. This is the only communication 
	// that happens between assimp & engine. Once data is in Model, there is no 
	// connection of it with assimp but rather it will be used by GameObject class
	// by assigning unique ID & name to it for further usage.
}

//////////////////////////////////////////////////////////////////////////
AssimpLoader::~AssimpLoader()
{
	// TO DO : 
	// We now can load Assimp mesh in this version. WE can actually load
	// mesh at 0th index only, WE need to write a code that will take care
	// loading all the sub-meshes in the assimp scene & load complete mesh
	// if it has sub-meshes. [DONE]

	// Also, right now only Diffuse Texture is loaded, we need to write code 
	// that supports rest of the textures from the mesh. [DONE]

	// Currently, Assimp creates a Model data structure, we need to make sure,
	// once Model object is created, we assign it to GameObject Class, where 
	// an unique ID & unique Game Asset name will be assigned to it. [DONE]

	// Only GameObject class interacts with Scene, rest of the stuff happens 
	// in the background. [DONE]
}

//////////////////////////////////////////////////////////////////////////
bool	AssimpLoader::LoadAssimpMesh(const std::string& fileName, const std::string& shaderName, Model** obj)
{
	HRESULT Hr = 0;
	LPDIRECT3DDEVICE9 device = RenderDevice::GetInstance()->getDevice();

	// Fill in model data
	(*obj)->m_sFilePath = fileName;
	(*obj)->m_pFxShader->CreateShader(shaderName);

	Assimp::Importer importer;
	const aiScene*	scene = importer.ReadFile( fileName, aiProcessPreset_TargetRealtime_Fast |
														 aiProcess_ConvertToLeftHanded);

	if(!scene)
	{
		LogManager::GetInstance()->WriteToFile(fileName.c_str(), " Assimp Scene Load FAILED");
		return false;
	}

	// Grab the first mesh!
	if(scene->HasMeshes())
	{
		// Fill in model data
		(*obj)->m_iNumMeshes = scene->mNumMeshes;

		for (unsigned int i = 0 ; i< scene->mNumMeshes ; i++)
		{
			aiMesh* tmpMesh = scene->mMeshes[i];
			if(!tmpMesh)
			{
				LogManager::GetInstance()->WriteToFile("<------ ERROR -----> ");
				LogManager::GetInstance()->WriteToFile(tmpMesh->mName.C_Str(), (int)i);
				return false;
			}

			// Extract Texture information per sub-mesh!	
			LPDIRECT3DTEXTURE9 tempDiffTex = NULL;
			
			aiMaterial* tmpMaterial = scene->mMaterials[tmpMesh->mMaterialIndex];

			// Check for diffuse material type only for time being!
			if(tmpMaterial && tmpMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				// Fill in model data
				(*obj)->m_bHasTexcoods = true;

				aiString diffuseTextureName;
				if( aiReturn_SUCCESS == tmpMaterial->GetTexture( aiTextureType_DIFFUSE, 
																 0, 
																 &diffuseTextureName,
																NULL, NULL, NULL, NULL, NULL))
				{
					// Map texture name with directory path & get the absolute texture path!
					std::string direcroyPath = Helper::ExtractDirectory(fileName);
					std::string absTexPath = direcroyPath.append(diffuseTextureName.data);

					Hr = D3DXCreateTextureFromFile(	device, 
													absTexPath.c_str(),
													&(tempDiffTex));

					if(S_OK != Hr)
					{
						LogManager::GetInstance()->WriteToFile("<------ ERROR -----> ");
						LogManager::GetInstance()->WriteToFile(diffuseTextureName.C_Str(), (int)i);
						LogManager::GetInstance()->WriteToFile("Loading default texture...");

						// Darn! Could not extract proper mesh texture, 
						// allow mesh loading with Default Texture
						Hr = D3DXCreateTextureFromFile(device, "Data\\ASSIMP\\Default.png", &(tempDiffTex));

						if(S_OK != Hr)
						{
							LogManager::GetInstance()->WriteToFile("ERROR ---> ", "Default Texture Load FAILED");
							return false;
						}
					}
				}
			}

			if (!tempDiffTex)
			{
				// Load Default Texture
				Hr = D3DXCreateTextureFromFile(device, "Data\\ASSIMP\\Default.png", &(tempDiffTex));

				if(S_OK != Hr)
				{
					LogManager::GetInstance()->WriteToFile("ERROR ---> ", "Default Texture Load FAILED");
					return false;
				}
			}

			// push into the main model's data structure
			(*obj)->m_vecDiffTex.push_back(tempDiffTex);
		

			// create temp index & vertex buffers...
			LPDIRECT3DVERTEXBUFFER9	tempVB = NULL;
			LPDIRECT3DINDEXBUFFER9	tempIB = NULL;

			//ZeroMemory(&(tempVB), tmpMesh->mNumVertices * sizeof(VertexPNT));
			Hr = device->CreateVertexBuffer(tmpMesh->mNumVertices * sizeof(VertexPNT),
											D3DUSAGE_WRITEONLY,
											D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
											D3DPOOL_DEFAULT,
											&(tempVB),
											NULL );

			if(S_OK != Hr)
			{
				LogManager::GetInstance()->WriteToFile("ERROR ---> ", "VB creation FAILED!");
				return false;
			}

			UINT nextVertex = 0;
			void* vertices = NULL;
			VertexPNT* tempVERT = NULL;

			(tempVB)->Lock(0, tmpMesh->mNumVertices * sizeof(VertexPNT), (void**)&vertices, 0);
			tempVERT = new VertexPNT[tmpMesh->mNumVertices];

			for (int i = 0 ; i < tmpMesh->mNumVertices ; i++)
			{
				tempVERT[nextVertex].pos = D3DXVECTOR3(tmpMesh->mVertices[i].x, tmpMesh->mVertices[i].y, tmpMesh->mVertices[i].z);
				tempVERT[nextVertex].normal = D3DXVECTOR3(tmpMesh->mNormals[i].x, tmpMesh->mNormals[i].y, tmpMesh->mNormals[i].z);
				if (tmpMesh->HasTextureCoords(0))
				{
					tempVERT[nextVertex].tex0 = D3DXVECTOR2(tmpMesh->mTextureCoords[0][i].x, tmpMesh->mTextureCoords[0][i].y);
				}
				else
				{
					tempVERT[nextVertex].tex0 = D3DXVECTOR2(0.0f, 0.0f);
				}

				nextVertex++;
			}

			memcpy(vertices, tempVERT, tmpMesh->mNumVertices * sizeof(VertexPNT));
			(tempVB)->Unlock();

			(*obj)->m_vecVB.push_back(tempVB);

			void* indices = 0;
			Hr = device->CreateIndexBuffer(	3 * tmpMesh->mNumFaces * sizeof(WORD),
											D3DUSAGE_WRITEONLY,
											D3DFMT_INDEX16,
											D3DPOOL_DEFAULT,
											&(tempIB),
											NULL);

			if(S_OK != Hr)
			{
				LogManager::GetInstance()->WriteToFile("ERROR ---> ", "IB Creation FAILED!");
				return false;
			}

			UINT nextIndex = 0;
			WORD*	tempINDEX = NULL;

			tempINDEX = new WORD[3 * tmpMesh->mNumFaces];

			for(UINT j = 0; j < tmpMesh->mNumFaces; j++)
			{
				tempINDEX[nextIndex++] = tmpMesh->mFaces[j].mIndices[0];
				tempINDEX[nextIndex++] = tmpMesh->mFaces[j].mIndices[1];
				tempINDEX[nextIndex++] = tmpMesh->mFaces[j].mIndices[2];
				//attributeBuffer[n] = meshes[mesh_id]->mMaterialIndex;
			}

			(tempIB)->Lock(0 , 3 * tmpMesh->mNumFaces * sizeof(WORD), (void**)&indices, 0);
			memcpy(indices, tempINDEX, 3 * tmpMesh->mNumFaces * sizeof(WORD));
			(tempIB)->Unlock();

			(*obj)->m_vecIB.push_back(tempIB);
			(*obj)->m_vecVertexCount.push_back(tmpMesh->mNumVertices);
			(*obj)->m_vecFaceCount.push_back(tmpMesh->mNumFaces);
			(*obj)->m_vecIndexCount.push_back(3 * tmpMesh->mNumFaces);
		}
	}

	return true;
}