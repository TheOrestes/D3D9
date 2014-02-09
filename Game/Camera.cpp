#include "Camera.h"
#include "../InputSystem/InputDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/Screenshot.h"
#include "../Renderables/BruteForceTerrain.h"
#include "../Scene/Scene.h"

//------------------------------------------------------------------------
// Purpose  : GLOBALS
//------------------------------------------------------------------------
const float CAMERA_SPEED = 0.0002f;
const float ORBIT_VIEW_MULTIPLIER = 50.0f;
const float CAMERA_ROATION_BALANCE_HORIZ = 20000.0f;
const float CAMERA_ROATION_BALANCE_VERT = 2000.0f;
const float CAMERA_ZOOM_MULTIPLIER = 5.0f;
const float CAMERA_ZOOM_ENHANCER = 0.01f;

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Camera::Camera()
{
	camType		=	ORBIT;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Camera::~Camera()
{
	LogManager::GetInstance()->WriteToFile("Camera Removed...");
}

//------------------------------------------------------------------------
// Purpose  : Initialize Camera
//------------------------------------------------------------------------
void	Camera::Initialize()
{
	
	if (camType == ORBIT)
	{
		//m_Position	= D3DXVECTOR3(-41.0f, 425.0f, -6.0f);
		//m_LookAt	= D3DXVECTOR3(13.0f, 398.0f, 57.0f);
		m_Position	= D3DXVECTOR3(0.0f, 350.0f, -150.0f);
		m_LookAt	= D3DXVECTOR3(0.0f, 300.0f, 0.0f);
		m_Up		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_fov		= D3DX_PI/4.0f;
	}
	
	LogManager::GetInstance()->WriteToFile("Camera Initialized");
}

//------------------------------------------------------------------------
// Purpose  : Move Camera 
//------------------------------------------------------------------------
void Camera::MoveCam(float speed)
{
	// Get the view vector
	D3DXVECTOR3 ViewVec = m_LookAt - m_Position;

	if (camType == ORBIT)
	{
		// Update forward & backward movement!
		m_Position.x	=	m_Position.x + ViewVec.x * speed*ORBIT_VIEW_MULTIPLIER;
		m_Position.z	=	m_Position.z + ViewVec.z * speed*ORBIT_VIEW_MULTIPLIER;
		m_Position.y	=	m_Position.y + ViewVec.y * speed*ORBIT_VIEW_MULTIPLIER;

		m_LookAt.x		=	m_LookAt.x + ViewVec.x * speed*ORBIT_VIEW_MULTIPLIER;	
		m_LookAt.z		=	m_LookAt.z + ViewVec.z * speed*ORBIT_VIEW_MULTIPLIER;
		m_LookAt.y		=	m_LookAt.y + ViewVec.y * speed*ORBIT_VIEW_MULTIPLIER;
	}
}

//------------------------------------------------------------------------
// Purpose  : Rotate Camera View
//------------------------------------------------------------------------
void Camera::RotateCam(float speed)
{
	// Get the view vector
	D3DXVECTOR3 ViewVec = m_LookAt - m_Position;

	m_LookAt.x	=	(float)(m_Position.x + cos(speed)*ViewVec.x - sin(speed)*ViewVec.z);
	m_LookAt.y	=	(float)(m_Position.y + sin(speed)*ViewVec.y + cos(speed)*ViewVec.y);
	m_LookAt.z	=	(float)(m_Position.z + sin(speed)*ViewVec.x + cos(speed)*ViewVec.z);
}

//------------------------------------------------------------------------
// Purpose  : Camera Strafe functionality
//------------------------------------------------------------------------
void Camera::StrafeCam(float speed)
{
	// Get the view vector
	D3DXVECTOR3 ViewVec = m_LookAt - m_Position;

	// Get Orthogonal vector LookAt Vector
	D3DXVECTOR3 ViewOrthoVec;

	ViewOrthoVec.x = -ViewVec.z;
	ViewOrthoVec.z = ViewVec.x;

	if (camType == ORBIT)
	{
		m_Position.x	=	m_Position.x - ViewOrthoVec.x*speed*ORBIT_VIEW_MULTIPLIER;
		m_Position.z	=	m_Position.z - ViewOrthoVec.z*speed*ORBIT_VIEW_MULTIPLIER;

		m_LookAt.x		=	m_LookAt.x - ViewOrthoVec.x*speed*ORBIT_VIEW_MULTIPLIER;
		m_LookAt.z		=	m_LookAt.z - ViewOrthoVec.z*speed*ORBIT_VIEW_MULTIPLIER;
	}
}

//------------------------------------------------------------------------
// Purpose  : handle mouse based movements!
//------------------------------------------------------------------------
void Camera::MouseMovCam(int wndWidth, int wndHeight)
{
	POINT mousePos;
	int mid_x = wndWidth  >> 1;		// bit Shift right to get half .. 								
	int mid_y = wndHeight >> 1;		// divide is costly!!
	
	float angle_y = 0.0f;
	float angle_z = 0.0f;

	GetCursorPos(&mousePos);
	if ((mousePos.x == mid_x) && (mousePos.y == mid_y))
		return;

	//SetCursorPos(mid_x, mid_y);		// Set cursor in the center of the screen!
	

	// Get the direction from the mouse cursor & set reasonable speed!
	angle_y = (float)((mid_x - mousePos.x))/CAMERA_ROATION_BALANCE_HORIZ;
	angle_z = (float)((mid_y - mousePos.y))/CAMERA_ROATION_BALANCE_VERT;

	// higher the value, faster the camera looks around!
	m_LookAt.y += angle_z * 100;

	// limit the rotation around X-axis!
	//if((m_LookAt.y - m_Position.y) > 145)  m_LookAt.y = m_Position.y + 50;
	//if((m_LookAt.y - m_Position.y) <-145)  m_LookAt.y = m_Position.y - 50;

	RotateCam(angle_y); // Rotate	
}

//------------------------------------------------------------------------
// Purpose  : Reset Camera
//------------------------------------------------------------------------
void	Camera::ResetCamera()
{
	Initialize();
}


//------------------------------------------------------------------------
// Purpose  : Update Camera
//------------------------------------------------------------------------
void Camera::Update(float dt)
{
	static float zoomAngle = 0.0f;

	if (InputDevice::GetInstance()->KeyDown(DIK_W))
	{
		MoveCam(CAMERA_SPEED * dt);
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_S))
	{
		MoveCam(-CAMERA_SPEED * dt);
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_A))
	{
		StrafeCam(-CAMERA_SPEED * dt);
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_D))
	{
		StrafeCam(CAMERA_SPEED * dt);
	}

	if (InputDevice::GetInstance()->MouseButtonDown(1))		// Rotate only
	{														// on Right Click!
		MouseMovCam(1280,1024);
	}

	if (InputDevice::GetInstance()->MouseButtonDown(2))		// Middle Click Zoom In
	{		
		// Zoom in Interpolation
		if (m_fov <= D3DX_PI / 8.0f)
			m_fov = D3DX_PI / 8.0f;
		else
			m_fov -= CAMERA_ZOOM_MULTIPLIER * dt;
	}
	else
	{
		// Zoom out interpolation
		if (m_fov < D3DX_PI / 2.0f)
			m_fov += CAMERA_ZOOM_MULTIPLIER * dt;
		else
			m_fov = (D3DX_PI / 2.0f);
	}
	
	if (InputDevice::GetInstance()->KeyDown(DIK_F))
	{
		camType = FIRST_PERSON;
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_T))
	{
		camType = THIRD_PERSON;
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_O))
	{
		camType = ORBIT;
	}

	if (InputDevice::GetInstance()->KeyDown(DIK_L))
	{
		D3DXVECTOR3 lookTo(10.0f, 100.0f, -100.f);
		LookAt(lookTo);
	}
}

//------------------------------------------------------------------------
// Purpose  : This function is called automatically since we have registered
//			  as Observer to the InputManager.
//------------------------------------------------------------------------
void Camera::Update(InputState state)
{

	switch (state)
	{
	case MOUSE_LEFTCLICK:
		// Zoom in Interpolation
		if (m_fov <= D3DX_PI / 8.0f)
			m_fov = D3DX_PI / 8.0f;
		else
			m_fov -= CAMERA_ZOOM_MULTIPLIER * CAMERA_ZOOM_ENHANCER;

		break;

	case MOUSE_RIGHTCLICK:
		// Zoom out interpolation
		if (m_fov < D3DX_PI / 2.0f)
			m_fov += CAMERA_ZOOM_MULTIPLIER * CAMERA_ZOOM_ENHANCER;
		else
			m_fov = (D3DX_PI / 2.0f);
		
		break;

	case MOUSE_MIDDLECLICK:
		MouseMovCam(800,600);
		break;

	case KEY_W:
		MoveCam(CAMERA_SPEED);
		break;

	case KEY_A:
		StrafeCam(-CAMERA_SPEED);
		break;

	case KEY_S:
		MoveCam(-CAMERA_SPEED);
		break;

	case KEY_D:
		StrafeCam(CAMERA_SPEED);
		break;

	case KEY_C:
		ResetCamera();
		break;
	}
}

//------------------------------------------------------------------------
// Purpose  : Look at 3D Position in Space!
//------------------------------------------------------------------------
void Camera::LookAt( const D3DXVECTOR3& lookAtPos )
{
	m_LookAt = lookAtPos;

	// recalculate right & up vector
// 	D3DXVECTOR3 newRight, newUp;
// 	D3DXVECTOR3	dummyUp(0.0f, 1.0f, 0.0f);
// 	D3DXVec3Cross(&newRight, &dummyUp, &m_LookAt);
// 	D3DXVec3Cross(&newUp, &m_LookAt, &newRight);
// 
// 	m_Up = newUp;
}

