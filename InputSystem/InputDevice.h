// ***************************************************************
//  InputDevice     
//  -------------------------------------------------------------
//  Purpose :  Basically handles Input system for the game through
//			   DirectInput System.
// ***************************************************************


#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <vector>
#include <map>
#include "ISubject.h"
#include "IObserver.h"

class InputDevice : public ISubject
{
public:

	virtual ~InputDevice();

	// Singleton Instance
	static InputDevice* GetInstance()
	{
		static InputDevice inst;
		return &inst;
	}

	// Implement the Interface
	void	registerObserver(IObserver* observer);
	void	removeObserver(IObserver* observer);
	void	notifyObserver(InputState state);
	
	bool	Initialize(HWND hwnd, HINSTANCE hInstance);
	void	MapKeycodes();
	void	Poll();
	bool	KeyDown(char key);
	bool	MouseButtonDown(int button);
	void	ShutDown();
	float	MouseDx();
	float	MouseDy();
	float	MouseDz();

private:
	InputDevice(const InputDevice&);						// Explicitly disallow the use of 
	InputDevice operator=(const InputDevice&);				// compiler-generated functions 

	InputDevice();

	IDirectInput8*			m_pInput;

	IDirectInputDevice8*	m_pKeyboard;
	char					m_KeyboardState[256];

	IDirectInputDevice8*	m_pMouse;	
	DIMOUSESTATE2			m_MouseState;

	std::vector<IObserver*>  m_vecObserver;
	std::map<char, InputState>	m_keyMap;
};

#endif