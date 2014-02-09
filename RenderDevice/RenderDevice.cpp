#include "RenderDevice.h"
#include "PostProcess.h"
#include "../Misc/LogManager.h"
#include "../Misc/EngineStats.h"
#include "../Scene/Scene.h"
#include "../Game/Camera.h"
#include "../ShaderEngine/Shader.h"
#include "../GeometrySystem/Vertex.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : Globals
//------------------------------------------------------------------------
HWND	g_lostDeviceWindow;
Scene*	g_World;

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
RenderDevice::RenderDevice()
{
	m_pD3D				=	NULL;
	m_pD3DDevice		=	NULL;
	m_dwBehaviorFlags	=	0;
	m_bDeviceLost		=	false;

	m_pEffectPtr		=	NULL;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
RenderDevice::~RenderDevice()
{
	Shutdown();
}

//------------------------------------------------------------------------
// Purpose  : Check for HAL support, HWP support n stuff!
//------------------------------------------------------------------------
bool RenderDevice::CheckDeviceCompatibility(IDirect3D9* d3d9Object)
{
	//	Verify pixel format combo which we r using is for display is 
	//	supported by the hardware.
	HRESULT Hr = 0;
	D3DDISPLAYMODE mode;
	d3d9Object->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);

	if (m_bWindowed)
	{
		Hr = d3d9Object->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true);
		LogManager::GetInstance()->WriteToFile("Device Starting in Windowed mode...");
	} 
	else
	{
		Hr = d3d9Object->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
		LogManager::GetInstance()->WriteToFile("Device Starting in Fullscreen mode...");
	}
	
	if (Hr != S_OK)
	{
		return false;
	}

	// Now check for Hardware Vertex Processing
	D3DCAPS9 caps;
	d3d9Object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	DWORD devBehaviorFlag = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
	{
		devBehaviorFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		LogManager::GetInstance()->WriteToFile("Hardware Vertex Processing supported...");
	}
	else
	{
		devBehaviorFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		LogManager::GetInstance()->WriteToFile("Software vertex processing mode...");
	}

	// check if Pure device & HW TL supported
 	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE && devBehaviorFlag & D3DCREATE_HARDWARE_VERTEXPROCESSING)
 	{
 		devBehaviorFlag |= D3DCREATE_PUREDEVICE;
 		LogManager::GetInstance()->WriteToFile("Pure Device mode...");
 	}

	// DoD -> 20/12/2009 
	// Check for Vertex & Pixel shader support
	if (caps.VertexShaderVersion < D3DVS_VERSION(3,0))
	{
		return false;
	}
	else
	{
		LogManager::GetInstance()->WriteToFile("Vertex Shader Model 3.0 Supported");
	}

	if (caps.PixelShaderVersion < D3DPS_VERSION(3,0))
	{
		return false;
	}
	else
	{
		LogManager::GetInstance()->WriteToFile("Pixel Shader Model 3.0 Supported");
	}


	m_dwBehaviorFlags = devBehaviorFlag;

	
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Initialize renderer
//------------------------------------------------------------------------
bool RenderDevice::Initialize(HWND hwnd, bool isWindowed)
{
	// Grab Windows Dimensions
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// Acquire D3D9 Interface
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// Check for device compatibility
	if (!CheckDeviceCompatibility(m_pD3D))
	{
		LogManager::GetInstance()->WriteToFile("Screwed!!! ... Device not compatible!");
		return false;
	}

	// check for Windowed or FullScreen Mode
	m_bWindowed = isWindowed;
	
	// Fill out D3DPRESENT_PARAMETERS
	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = m_bWindowed;
	d3dpp.EnableAutoDepthStencil     = true;
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// Locks fps to 60!

	// Finally after shit load of drama, we create our device
	HRESULT Hr = 0;
	Hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, m_dwBehaviorFlags, &d3dpp, &m_pD3DDevice);

	if (Hr != S_OK)
	{
		LogManager::GetInstance()->WriteToFile("MEGA RAPE! ... device cannot be created!", __FILE__, __LINE__);
		return false;
	}

	// Save PRESENT_PARAMS & other valuable assets for LOST_DEVICE
	m_d3dpp = d3dpp;
	g_lostDeviceWindow = hwnd;
	// OnResetDevice();	
	LogManager::GetInstance()->WriteToFile("RenderDevice Initialized");

	// Vertex Declaration
	InitAllVertexDeclarations();

	// Init Post Process
	m_pEffectPtr = new PostProcess();
	m_pEffectPtr->PostProcessInit(m_pD3DDevice);

	return true;
}

//------------------------------------------------------------------------
// Purpose  : update rendering
//------------------------------------------------------------------------
void RenderDevice::Update(float dt)
{
}

//------------------------------------------------------------------------
// Purpose  : Do actual rendering
//------------------------------------------------------------------------
HRESULT RenderDevice::DoRendering()
{
	// Before we render, we need to make sure we haven't lost our device.
	// If we have, we'll need to restore it before we can continue.
	HRESULT Hr;

	// Test Co-op level to decide what to do based on the result.
	Hr = m_pD3DDevice->TestCooperativeLevel();

	if (Hr == D3DERR_DEVICELOST)
	{
		LogManager::GetInstance()->WriteToFile("D3DERR_DEVICELOST but WAIT");
		// The device has been lost but cannot be reset at this time. 
		// Therefore, rendering is not possible and we'll have to return 
		// and try again at a later time.
		// m_bDeviceLost = true;
		return S_OK;
	}
	else if(Hr == D3DERR_DEVICENOTRESET)
	{
		m_bDeviceLost = true;
		return S_OK;
	}

	if (FAILED(Hr))
		return Hr;

	if (m_bDeviceLost)
	{
		if (FAILED( Hr = OnResetDevice()))
			return Hr;

		m_bDeviceLost = false;
	}

	// Render the scene as Normal
	if (FAILED(Hr = Render()))
		return Hr;

	m_pD3DDevice->Present(0,0,0,0);

	// Make sure device is good & running
	Hr = m_pD3DDevice->TestCooperativeLevel();
	if ( Hr == D3DERR_DEVICELOST)
	{
		m_bDeviceLost = true;
		return S_OK;
	}

	if (FAILED(Hr))
		return Hr;

	return S_OK;
}

//------------------------------------------------------------------------
// Purpose  : Render!
//------------------------------------------------------------------------
HRESULT RenderDevice::Render()
{
	// Camera Stuff!	
	D3DXVECTOR3 Eye		=	Camera::GetInstance()->m_Position;
	D3DXVECTOR3 LookAt	=	Camera::GetInstance()->m_LookAt; 
	D3DXVECTOR3 Up		=	Camera::GetInstance()->m_Up; 
	float	    FOV		=	Camera::GetInstance()->m_fov;

	D3DXMatrixLookAtLH(&m_viewMatrix, &Eye, &LookAt, &Up );
	//m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_viewMatrix);// Apply Changes To The View

	//D3DXMatrixOrthoLH(&m_projMatrix, 1280.0f, 720.0f, 1.0f, 50000.0f);
	D3DXMatrixPerspectiveFovLH(&m_projMatrix, FOV, 1280.0f/720.0f, 0.1f, 10000.f);

	//m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_projMatrix);
	
	//-- Render World to a texture 
	m_pEffectPtr->RenderToTextureBegin(m_pD3DDevice);
	Scene::GetInstance()->Render();
	m_pEffectPtr->RenderToTextureEnd(m_pD3DDevice);
	
	m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xff123456, 1.0f, 0);

	m_pD3DDevice->BeginScene();

	// Apply post-processing here
	m_pEffectPtr->RenderProcessedQuad(m_pD3DDevice);

	// Render Engine stats
	EngineStats::GetInstance()->Render();

	m_pD3DDevice->EndScene();

	return S_OK;
}

//------------------------------------------------------------------------
// Purpose  : Shutdown rendering
//------------------------------------------------------------------------
void RenderDevice::Shutdown()
{
	// Destroy all vertex declarations
	DestroyAllVertexDeclarations();

	LogManager::GetInstance()->WriteToFile("Shutting down Rendering device...");

	SAFE_DELETE(m_pEffectPtr);
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_pD3D);
	
	LogManager::GetInstance()->WriteToFile("RenderDevice Shutdown Successful!");
}



//------------------------------------------------------------------------
// Purpose  : Basically stuff we need to do once device is lost. This 
//			  includes, saving vertex buffer, index buffers, textures,
//			  transformations, render states, any meshes loaded or lights.
//------------------------------------------------------------------------
void RenderDevice::OnDeviceLost()
{
	LogManager::GetInstance()->WriteToFile("Device Lost...");	

	
}

//------------------------------------------------------------------------
// Purpose  : Recreate Everything that we deleted on LostDevice state! 
//------------------------------------------------------------------------
HRESULT RenderDevice::OnResetDevice()
{
	LogManager::GetInstance()->WriteToFile("Device is Reset...");

	HRESULT	Hr;

	Hr = m_pD3DDevice->Reset(&m_d3dpp);

	if (Hr == S_OK)
	{
		Initialize(g_lostDeviceWindow, m_bWindowed);
	}
	else
	{
		Hr = -1;
	}

	return Hr;
}

//------------------------------------------------------------------------
// Purpose  : Toggle FullScreen-Windowed mode
//------------------------------------------------------------------------
void RenderDevice::ToggleWindowedMode()
{
	m_bDeviceLost = true;
	if (IsWindowedMode())
	{
		m_bWindowed = false;
	}
	else 
	{
		m_bWindowed = true;
	}
}

//------------------------------------------------------------------------ 
// Purpose  : Get-Set View & Projection Matrices
//------------------------------------------------------------------------
D3DXMATRIX*	RenderDevice::GetGlobalProjectionMatrix()
{
	return &m_projMatrix;
}

D3DXMATRIX*	RenderDevice::GetGlobalViewMatrix()
{
	return &m_viewMatrix;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RenderDevice::SetGlobalViewMatrix( const D3DXMATRIX& viewMat )
{
	m_viewMatrix = viewMat;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RenderDevice::SetGlobalProjectionMatrix( const D3DXMATRIX& projMat )
{
	m_projMatrix = projMat;
}

//------------------------------------------------------------------------
// Purpose  : MsgProc for RenderDevice
//------------------------------------------------------------------------
HRESULT RenderDevice::MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_SYSKEYDOWN:
		{
			switch( wParam )
			{
				case VK_RETURN:
				{
						
 					// Toggle full screen upon alt-enter 
					DWORD dwMask = (1 << 29);
 					if( (lParam & dwMask) != 0 ) // Alt is down also
 					{
 						// Toggle the full screen/window mode
 						//DXUTPause( true, true );
 						ToggleWindowedMode();
 						//DXUTPause( false, false );
 						return 0;
 					}
				}
				break;
			}
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}


