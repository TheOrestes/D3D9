// ***************************************************************
//  SpotLightObject			
//  -------------------------------------------------------------
//  Purpose :	Basic Point Light Object
// ***************************************************************

#ifndef SPOT_LIGHT_OBJECT
#define SPOT_LIGHT_OBJECT

class GameObject;

class SpotLightObject : public GameObject
{
public:
	SpotLightObject ();
	virtual ~SpotLightObject();
};

#endif