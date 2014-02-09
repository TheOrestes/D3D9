// ***************************************************************
//  LightObject			18/09/2013
//  -------------------------------------------------------------
//  Purpose :	Basic Light Object
//  -------------------------------------------------------------
//  DoD -> Copyright (C) 2013 - All Rights Reserved
// ***************************************************************

#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

#include <d3d9.h>
#include <d3dx9.h>
#include "../GeometrySystem/GameObject.h"

enum LightType
{
	DIRECTIONAL,
	OMNI,
	SPOT
};

class LightObject : public GameObject
{
public:
	LightObject();
	virtual ~LightObject();

	// Override Interface Functions!
	virtual void			Init(){};
	virtual	void			Update(float dt){};
	virtual void			Render(){};
	virtual void			Kill(){};

	// GETTER
	LightType				getLightType() { return m_eType; }
	virtual D3DXCOLOR		getLightColor(){ return m_Color; }
	virtual D3DXVECTOR3		getLightPosition(){ return m_vecPosition; }
	virtual D3DXVECTOR3		getLightDirection(){ return m_vecDirection; }
	virtual float			getLightIntensity(){ return m_fIntensity; }
	virtual float			getLightRange(){ return m_fRange; }
	virtual float			getLightAtten0(){ return m_fAttenuation0; }
	virtual float			getLightAtten1(){ return m_fAttenuation1; }
	virtual float			getLightAtten2(){ return m_fAttenuation2; }
	virtual float			getOuterConeAngle(){ return m_fOuterConeAngle; }
	virtual float			getInnerConeAngle(){ return m_fInnerConeAngle; }
	virtual float			getConeAngleFalloff(){ return m_fConeAngleFalloff; }

	// SETTER
	virtual void			setLightType(int type);
	virtual void			setLightColor(D3DXCOLOR col); 
	virtual void			setLightPosition(D3DXVECTOR3 pos);
	virtual void			setLightDirection(D3DXVECTOR3 dir);
	virtual void			setLightIntensity(float intensity);
	virtual void			setLightRange(float range);
	virtual void			setLightAtten0(float att0);
	virtual void			setLightAtten1(float att1);
	virtual void			setLightAtten2(float att2);
	virtual void			setOuterConeAngle(float ang);
	virtual void			setInnerConeAngle(float ang);
	virtual void			setConeAngleFalloff(float falloff);

protected:
	LightType				m_eType;
	D3DXCOLOR				m_Color;
	D3DXVECTOR3				m_vecPosition;
	D3DXVECTOR3				m_vecDirection;
	float					m_fIntensity;
	float					m_fRange;
	float					m_fAttenuation0;
	float					m_fAttenuation1;
	float					m_fAttenuation2;	
	float					m_fOuterConeAngle;
	float					m_fInnerConeAngle;
	float					m_fConeAngleFalloff;
};

#endif