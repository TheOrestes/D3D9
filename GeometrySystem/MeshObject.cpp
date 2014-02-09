
#include "MeshObject.h"
#include "../GeometrySystem/AssimpLoader.h"
#include "../GeometrySystem/Model.h"
#include "../Misc/LogManager.h"
#include "../Misc/EngineStats.h"

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
MeshObject::MeshObject()
{
	m_pModel	=	new Model();
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
MeshObject::~MeshObject()
{
	Kill();
}

//----------------------------------------------------------------------------------------
// Purpose  : Init
//----------------------------------------------------------------------------------------
void	MeshObject::Init()
{
	if(!LoadModel(m_strFilePath, m_strShader))
	{
		LogManager::GetInstance()->WriteToFile(m_strFilePath.c_str(), "LoadModel FAILED!");
		return;
	}
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
bool	MeshObject::LoadModel(const std::string& filename, const std::string& shadername)
{
	if(!AssimpLoader::GetInstance()->LoadAssimpMesh(filename, shadername, &m_pModel))
	{
		LogManager::GetInstance()->WriteToFile(filename.c_str(), "LoadModel FAILED");
		return false;
	}

	// Load Model's Environment Map
	if(m_pModel && !m_pModel->LoadEnvironmentMap(m_strEnvMap))
	{
		LogManager::GetInstance()->WriteToFile(m_strEnvMap.c_str(), "Loading FAILED");
		return false;
	}	

	// 12 Oct 2013 [DoD]
	// Assign Model Specific Data!
	m_pModel->SetShaderTechnique(m_strShader);
	m_pModel->SetMaterialProperties(m_colEmissive, m_colAmbient, m_colDiffuse, m_colSpecular);

	// Update Engine Stats!
	DWORD	_vert = (DWORD)m_pModel->GetVertexCount();
	EngineStats::GetInstance()->addVertices(_vert);

	DWORD	_tris = (DWORD)m_pModel->GetTriangleCount();
	EngineStats::GetInstance()->addTriangles(_tris);

	return true;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::setModelPath(const std::string& path)
{
	m_strFilePath = path;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::setModelShader(const std::string& shader)
{
	m_strShader = shader;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::setObjectPos(const D3DXVECTOR3& pos)
{
	m_vecPos = pos;
	m_pModel->Translate(m_vecPos);
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::setObjectRot(const D3DXVECTOR3& rot, float angle)
{
	m_vecRot = rot;
	m_pModel->Rotate(m_vecRot, angle);
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::setObjectScale(const D3DXVECTOR3& sc)
{
	m_vecScale = sc;
	m_pModel->Scale(m_vecScale);
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void MeshObject::setModelEmissiveCol(const D3DXVECTOR4& col )
{
	m_colEmissive = col;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void MeshObject::setModelAmbientCol(const D3DXVECTOR4& col )
{
	m_colAmbient = col;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void MeshObject::setModelDiffuseCol(const D3DXVECTOR4& col )
{
	m_colDiffuse = col;
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void MeshObject::setModelSpecularCol(const D3DXVECTOR4& col )
{
	m_colSpecular = col;
}


//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::Update(float dt)
{
	m_pModel->Update(dt);
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::Render()
{
	m_pModel->Render();
}

//----------------------------------------------------------------------------------------
// Purpose  : 
//----------------------------------------------------------------------------------------
void	MeshObject::Kill()
{
	SAFE_DELETE(m_pModel);
}


