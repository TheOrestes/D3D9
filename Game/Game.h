// ***************************************************************
//  Game     
//  -------------------------------------------------------------
//  Purpose :	Core class. Mainly starts Timer. Initializes Engine
//				class & in Future Console as well. 
// ***************************************************************

#include <Windows.h>

#ifndef GAME_H
#define GAME_H

class Engine;

class Game	
{
public:
	
	virtual ~Game();

	// Singleton Instance
	static Game* GetInstance()
	{
		static Game instance;
		return &instance;
	}
	
	bool Initialize(HWND hwnd, HINSTANCE hInstance);
	void Shutdown();
	void Tick();
	void FrameRender();
	LRESULT HandleMessages(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	Engine*		m_pEngine;

private:
	Game(const Game&);					// Explicitly disallow the use of 
	Game operator=(const Game&);		// compiler-generated functions 

	Game();
};


#endif