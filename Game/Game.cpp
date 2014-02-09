#include "Game.h"
#include "../Engine/Engine.h"
#include "../RenderDevice/RenderDevice.h"

//------------------------------------------------------------------------
// Purpose  : Globals 
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Game::Game()
{
	
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Game::~Game()
{

}

//------------------------------------------------------------------------
// Purpose  : Initialize Game, birthplace for everything
//------------------------------------------------------------------------
bool Game::Initialize(HWND hwnd, HINSTANCE hInstance)
{
	// Initialize main Engine
	m_pEngine = Engine::GetInstance();
	
	if (!m_pEngine->Initialize(hwnd, hInstance))
	{
		return false;
	}
	
	return true;
}

//------------------------------------------------------------------------
// Purpose  : Shutdown Game, Everything rests in peace from here.
//------------------------------------------------------------------------
void Game::Shutdown()
{
	if (m_pEngine)
	{
		m_pEngine->Shutdown();
		m_pEngine = NULL;
	}
}

//------------------------------------------------------------------------
// Purpose  : Game Tick, Heartbeat for the game is provided from here.
//------------------------------------------------------------------------
void Game::Tick()
{
	// Update Engine stuff 
	if (!m_pEngine)
		return;

	m_pEngine->Update();
	
	// Update Rendering
	RenderDevice::GetInstance()->DoRendering();
}

//------------------------------------------------------------------------
// Purpose  : FrameRender, well, renders everything or starts from here.
//------------------------------------------------------------------------
void Game::FrameRender()
{
// 	if (m_pWorld)
// 	{
// 		m_pWorld->Render(camera);
// 	}
// 
// 	if (!IsInGame())
// 	{
// 		Gui::Instance()->Render();
// 	}
}

//------------------------------------------------------------------------
// Purpose  : Handle message loop, call default WndProc n stuff.
//------------------------------------------------------------------------
LRESULT Game::HandleMessages(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_pEngine)
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return m_pEngine->HandleMessages(hwnd, message, wParam, lParam);
}