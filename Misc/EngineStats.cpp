#include "EngineStats.h"
#include "../RenderDevice/RenderDevice.h"
#include "../InputSystem/InputDevice.h"
#include "../Game/Camera.h"
#include "../Misc/Helper.h"
#include <stdio.h>
#include <ctime>

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
EngineStats::EngineStats()
{
	m_Font			=	NULL;
	m_FPS			=	0.0f;
	m_msPerFrame	=	0.0f;	
	m_delta			=	0.0f;
	m_numTriangles	=	0;
	m_numVertices	=	0;
	m_iDays			=	0;
	m_iMonths		=	0;
	m_iYears		=	0;
	m_iHours		=	0;
	m_iMinutes		=	0;
	m_iSeconds		=	0;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
EngineStats::~EngineStats()
{
	SAFE_RELEASE(m_Font);
}

//------------------------------------------------------------------------
// Purpose  : Initialize Engine Stats System
//------------------------------------------------------------------------
bool EngineStats::Initialize()
{
	// Get Device first
	IDirect3DDevice9* device = RenderDevice::GetInstance()->getDevice();

	// create font
	HRESULT Hr = D3DXCreateFont( device, 
								 16, 
								 0, 
								 FW_NORMAL,
								 0, 
								 false, 
								 DEFAULT_CHARSET, 
								 OUT_TT_ONLY_PRECIS, 
								 ANTIALIASED_QUALITY, 
								 DEFAULT_PITCH | FF_DONTCARE, 
								 TEXT("Orator Std"), 
								 &m_Font);

	if (FAILED(Hr))
	{
		return false;
	}
	
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Vertices & Triangles Calculations
//------------------------------------------------------------------------
void	EngineStats::addTriangles(DWORD n)
{
	m_numTriangles += n;
}

void	EngineStats::subTriangles(DWORD n)
{
	m_numTriangles -= n;
}

void	EngineStats::addVertices(DWORD n)
{
	m_numVertices += n;
}

void	EngineStats::subVertices(DWORD n)
{
	m_numVertices -= n;
}

void	EngineStats::setTriCount(DWORD n)
{
	m_numTriangles = n;
}

void	EngineStats::setVertexCount(DWORD n)
{
	m_numVertices = n;
}

//------------------------------------------------------------------------
// Purpose  : Update Engine stats
//------------------------------------------------------------------------
void EngineStats::Update(float dt)
{
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	// increment Frame counter
	numFrames += 1.0f;

	// store delta for debugging
	m_delta = dt;

	// Accumulate how much time has passed
	timeElapsed += dt;

	// Has one second passed?
	if (timeElapsed >= 1.0f)
	{
		// FPS is numFrames/timeElapsed, here timeElapsed = 1.0f
		m_FPS = numFrames;

		// Avg time in Millisecond it took to render a single frame
		m_msPerFrame = 1000.0f/m_FPS;

		// reset counter for next second
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
}

//------------------------------------------------------------------------
// Purpose  : Render the Engine Stats on screen
//------------------------------------------------------------------------
void EngineStats::Render()
{
	// make static so memmory not allocated every frame
	static char buffer[512];
	
	D3DXVECTOR3 camPos = Camera::GetInstance()->m_Position;
	D3DXVECTOR3 camLook = Camera::GetInstance()->m_LookAt;

	// Get Current System Date
	std::time_t tt = std::time(0);
	std::tm ttm = *std::localtime(&tt);
	m_iDays = ttm.tm_mday;
	m_iMonths = ttm.tm_mon+1;
	m_iYears = ttm.tm_year+1900;
	m_iHours = ttm.tm_hour;
	m_iMinutes = ttm.tm_min; 
	m_iSeconds = ttm.tm_sec;

	sprintf(buffer,	" FPS : %.2f\n"
		" DELTA	 : %.6f\n"
		" CamPos : %.2f %.2f %.2f\n"
		" CamLook: %.2f %.2f %.2f\n"
		//" Triangles : %d\n"
		//" Vertices : %d\n"
		" Time : %d/%d/%d    %d:%d:%d", m_FPS, 
										m_delta, 
										camPos.x, camPos.y, camPos.z, 
										camLook.x, camLook.y, camLook.z, 
									//	m_numTriangles, 
									//	m_numVertices,
										m_iDays, m_iMonths, m_iYears,
										m_iHours, m_iMinutes, m_iSeconds);
	

	RECT R = {40,40,0,0};
	
	// Render Stats
	m_Font->DrawText(0, (buffer), -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255,255,0));
}

//------------------------------------------------------------------------
// Purpose  : On Lost & Reset Device
//------------------------------------------------------------------------
void	EngineStats::onLostDevice()
{
	m_Font->OnLostDevice();
}

void	EngineStats::onResetDevice()
{
	m_Font->OnResetDevice();
}

