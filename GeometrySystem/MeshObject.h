
#ifndef MESH_OBJECT_H
#define MESH_OBJECT_H

#include "IObject.h"
#include "GameObject.h"
#include <string>

struct Model;

class MeshObject : public GameObject
{
public:
	MeshObject();
	virtual ~MeshObject();

	virtual void		Init();
	bool				LoadModel(const std::string& filename, const std::string& shadername);

	// GETTER
	inline Model*		getModel()			{ return m_pModel; }
	inline std::string	getModelPath()		{ return m_strFilePath; }
	inline std::string	getModelShader()	{ return m_strShader; }
	inline D3DXVECTOR4	getModelEmissiveCol() { return m_colEmissive; }
	inline D3DXVECTOR4	getModelAmbientCol()  { return m_colAmbient; }
	inline D3DXVECTOR4	getModelDiffuseCol()  { return m_colDiffuse; }
	inline D3DXVECTOR4	getModelSpecularCol() { return m_colSpecular; }

	// SETTER
	void				setModelPath(const std::string& path);
	void				setModelShader(const std::string& shader);
	void				setModelEmissiveCol(const D3DXVECTOR4& col);				
	void				setModelAmbientCol(const D3DXVECTOR4& col); 
	void				setModelDiffuseCol(const D3DXVECTOR4& col); 
	void				setModelSpecularCol(const D3DXVECTOR4& col);

	// Transformations
	virtual void		setObjectPos(const D3DXVECTOR3& pos);
	virtual void		setObjectRot(const D3DXVECTOR3& rot, float angle);
	virtual void		setObjectScale(const D3DXVECTOR3& sc);


	// Override Interface Functions!
	virtual	void		Update(float dt);
	virtual void		Render();

	virtual void		Kill();

private:
	Model*				m_pModel;

	std::string			m_strFilePath;
	std::string			m_strShader;

	D3DXVECTOR4			m_colEmissive;
	D3DXVECTOR4			m_colAmbient;
	D3DXVECTOR4			m_colDiffuse;
	D3DXVECTOR4			m_colSpecular;
};

#endif