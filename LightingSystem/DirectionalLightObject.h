// ***************************************************************
//  DiectionalLightObject			
//  -------------------------------------------------------------
//  Purpose :	Basic Directional Light Object
// ***************************************************************

#ifndef DIRECTIONAL_LIGHT_OBJECT
#define DIRECTIONAL_LIGHT_OBJECT

class GameObject;

class DirectionalLightObject : public GameObject
{
public:
	DirectionalLightObject();
	virtual ~DirectionalLightObject();

	virtual void		Update(float dt);

	// Setter 
	void				setLightDirection(const D3DXVECTOR3& dir);
	void				setLightColor(const D3DXVECTOR4& changeCol);
	void				setLightIntensity(float intensity);
	void				setLightChangeDir(const D3DXVECTOR3& changeDir);

	// Getter
	inline D3DXVECTOR4	getLightColor()		{ return m_vecLightCol; } 
	inline D3DXVECTOR3	getLightDirection()	{ return m_vecLightDir; }
	inline float		getLightIntensity()	{ return m_fIntensity;  }
	inline D3DXVECTOR3	getLightChangeDir() { return m_vecChangeDir;}

private:
	D3DXVECTOR4			m_vecLightCol;
	D3DXVECTOR3			m_vecLightDir;
	float				m_fIntensity;
	D3DXVECTOR3			m_vecChangeDir;
};

#endif