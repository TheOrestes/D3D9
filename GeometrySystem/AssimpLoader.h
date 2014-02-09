
#ifndef ASSIMP_LOADER_H
#define ASSIMP_LOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>
#include <vector>
#include <d3dx9.h>

struct Model;

class AssimpLoader
{
public:
	// We need only ONE loader at any given time!
	static AssimpLoader* GetInstance()
	{
		static AssimpLoader inst;
		return &inst;
	}

	~AssimpLoader();

	bool			LoadAssimpMesh(const std::string& filename, const std::string& shaderName, Model** obj);

private:
	AssimpLoader();

	std::vector<LPDIRECT3DVERTEXBUFFER9> m_vecVB;
	std::vector<LPDIRECT3DINDEXBUFFER9>  m_vecIB;
};

#endif