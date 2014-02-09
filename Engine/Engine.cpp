#include "Engine.h"
#include "Clock.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/LogManager.h"
#include "../Misc/EngineStats.h"
#include "../InputSystem/InputDevice.h"
#include "../Scene/Scene.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Engine::Engine()
{
	m_pClock = new Clock();
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Engine::~Engine()
{
	
}

//------------------------------------------------------------------------
// Purpose  : Initialize RenderDevice & Input & in future shader stuff
//------------------------------------------------------------------------
bool Engine::Initialize(HWND hwnd, HINSTANCE hInstance)
{
	// Start Engine Timer
	m_pClock->Initialize();

	// Initialize Log System
	if(!LogManager::GetInstance()->Initialize())
		return false;

	// Initialize RenderDevice
	if(!RenderDevice::GetInstance()->Initialize(hwnd, true))
		return false;

	// Initialize Engine Stats system
	if(!EngineStats::GetInstance()->Initialize())
		return false;

	// Initialize Input System
	if(!InputDevice::GetInstance()->Initialize(hwnd, hInstance))
		return false;

	// Initialize Game World
	if(!Scene::GetInstance()->Initialize(m_pClock))
		return false;

	// Initialize GUI system
	
	// Initialize Log System
	LogManager::GetInstance()->WriteToFile("Engine Initialized");

	// MessageBox(NULL,L"Engine",NULL,NULL);
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Shutdown Engine
//------------------------------------------------------------------------
void Engine::Shutdown()
{
	InputDevice::GetInstance()->ShutDown();
	Scene::GetInstance()->Shutdown();
	RenderDevice::GetInstance()->Shutdown();
	LogManager::GetInstance()->Shutdown();

	if(m_pClock)
		m_pClock = NULL;
}

//------------------------------------------------------------------------
// Purpose  : Update overall stuff happening.
//------------------------------------------------------------------------
void Engine::Update()
{
	// Get the TICK!
	float dt = m_pClock->GetTheTick();

	// Input->update();
	InputDevice::GetInstance()->Poll();

	// Update Engine stats
	EngineStats::GetInstance()->Update(dt);

	// Update Game World
	Scene::GetInstance()->Update(dt);

	// Update Rendering System
	RenderDevice::GetInstance()->Update(dt);
}

//------------------------------------------------------------------------
// Purpose  : Handle Messages
//------------------------------------------------------------------------
LRESULT Engine::HandleMessages(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (RenderDevice::GetInstance())
	{
		// Input->MsgProc()
		return RenderDevice::GetInstance()->MsgProc(hwnd, message, wParam, lParam);
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}