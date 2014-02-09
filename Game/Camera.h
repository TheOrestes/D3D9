// ***************************************************************
//  Camera     
//  -------------------------------------------------------------
//  Purpose : First person & Third Person Camera System
// ***************************************************************

#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>
#include "../InputSystem/IObserver.h"

class Screenshot;

class Camera : public IObserver
{
public:
	virtual ~Camera();

	// Singleton Instance
	static Camera* GetInstance()
	{
		static Camera inst;
		return &inst;
	}

	enum CameraType
	{
		FIRST_PERSON,
		THIRD_PERSON,
		ORBIT
	};

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_LookAt;
	D3DXVECTOR3 m_Up;

	float		m_fov;

	CameraType	camType;

	void		Initialize();
	void		StrafeCam(float speed);
	void		MoveCam(float speed);
	void		RotateCam(float speed);
	void		MouseMovCam(int wndWidth, int wndHeight);
	void		ResetCamera();

	void		LookAt(const D3DXVECTOR3& lookAtPos);

	void		Update(float dt);

	void		Update(InputState state);

private:
	Camera();

	Camera(Camera&);
	Camera operator=(Camera&);

	Screenshot*		pGrabScreen;
};

#endif