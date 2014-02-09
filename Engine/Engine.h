// ***************************************************************
//  Engine     
//  -------------------------------------------------------------
//  Purpose :	Initialize Shaders, Rendering, Input & if possible 
//				GUI system as well.
// ***************************************************************

#include <Windows.h>

#ifndef ENGINE_H
#define ENGINE_H

class Clock;

class Engine
{
public:
	virtual ~Engine();

	// Singleton Instance
	static Engine* GetInstance()
	{
		static Engine instance;
		return &instance;
	}

	bool Initialize(HWND hwnd, HINSTANCE hInstance);
	void Shutdown();
	void Update();
	LRESULT HandleMessages(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	Engine(const Engine&);					// Explicitly disallow the use of 
	Engine operator=(const Engine&);		// compiler-generated functions 

	Engine();

	Clock*		m_pClock;
};
#endif