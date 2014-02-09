// ***************************************************************
//  PointLightObject			
//  -------------------------------------------------------------
//  Purpose :	Basic Point Light Object
// ***************************************************************

#ifndef POINT_LIGHT_OBJECT
#define POINT_LIGHT_OBJECT

class GameObject;

class PointLightObject : public GameObject
{
public:
	PointLightObject ();
	virtual ~PointLightObject();

	virtual void		Init();
	virtual void		Update(float dt);
	virtual void		Render();
	virtual void		Kill();

	// Setter
	void				setLightColor(const D3DXVECTOR4& col); 
	void				setLightPosition(const D3DXVECTOR3& pos);
	void				setLightIntensity(float intensity);
	void				setLightRadius(float radius);
	void				setLightAtten(int atten);
	void				setLightVelocity(const D3DXVECTOR3& vel);

	// Getter
	inline D3DXVECTOR4	getLightColor()		{ return m_vecLightCol; } 
	inline D3DXVECTOR3	getLightPosition()	{ return m_vecLightPos; }
	inline float		getLightIntensity()	{ return m_fIntensity;  }
	inline float		getLightRadius()	{ return m_fRadius;     }
	inline int			getLightAtten()		{ return m_iAttenuation;}
	inline D3DXVECTOR3	getLightVelocity()	{ return m_vecVelocity; }

	
private:
	D3DXVECTOR4			m_vecLightCol;
	D3DXVECTOR3			m_vecLightPos;
	float				m_fRadius;
	float				m_fIntensity;
	int					m_iAttenuation;

	// Point Light Mesh
	LPD3DXMESH			m_pMesh;
	LPD3DXEFFECT		m_pEffect;

	// Extra data
	D3DXVECTOR3			m_vecVelocity;
};

#endif