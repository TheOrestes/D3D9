// ***************************************************************
//  RenderDevice     
//  -------------------------------------------------------------
//  Purpose :	Handle D3D device operations, look after rendering.
// ***************************************************************

#include <d3d9.h>
#include <d3dx9.h>

#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

class PostProcess;

class RenderDevice
{
public:
	
	virtual ~RenderDevice();
	
	// Singleton Instance
	static RenderDevice* GetInstance()
	{
		static RenderDevice inst;
		return &inst;
	}

	bool		CheckDeviceCompatibility(IDirect3D9* d3d9Object);

	bool		Initialize(HWND hwnd, bool isWindowed);
	void		Update(float dt);
	HRESULT		DoRendering();
	void		Shutdown();

	void		OnDeviceLost();
	HRESULT		OnResetDevice();

	HRESULT		Render();

	void		ToggleWindowedMode();
	inline bool IsWindowedMode(){ return m_bWindowed; }
	
	inline IDirect3DDevice9* getDevice() { return m_pD3DDevice; }
	D3DXMATRIX*	GetGlobalViewMatrix();
	D3DXMATRIX*	GetGlobalProjectionMatrix();

	void		SetGlobalViewMatrix(const D3DXMATRIX& viewMat);
	void		SetGlobalProjectionMatrix(const D3DXMATRIX& projMat);

	HRESULT		MsgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	bool		m_bDeviceLost;

private:
	RenderDevice();

	RenderDevice(const RenderDevice&);					// Explicitly disallow the use of 
	RenderDevice operator=(const RenderDevice&);		// compiler-generated functions 

	IDirect3D9*				m_pD3D;
	IDirect3DDevice9*		m_pD3DDevice;
	bool					m_bWindowed;
	DWORD					m_dwBehaviorFlags;
	D3DPRESENT_PARAMETERS	m_d3dpp;

	D3DXMATRIX				m_viewMatrix;
	D3DXMATRIX				m_projMatrix;

	PostProcess*			m_pEffectPtr;
};

#endif