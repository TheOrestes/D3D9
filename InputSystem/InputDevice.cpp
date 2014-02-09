#include "InputDevice.h"
#include "InputState.h"
#include "../Misc/LogManager.h"
#include "../Misc/Helper.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
InputDevice::InputDevice()
{
	m_vecObserver.clear();
	m_keyMap.clear();
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
InputDevice::~InputDevice()
{
	ShutDown();
}

//------------------------------------------------------------------------
// Purpose  : Map dinput8 key codes to game key codes in InputState enum
//------------------------------------------------------------------------
void InputDevice::MapKeycodes()
{
	// Keyboard Mapping
	m_keyMap.insert(std::make_pair(DIK_W, KEY_W));
	m_keyMap.insert(std::make_pair(DIK_A, KEY_A));
	m_keyMap.insert(std::make_pair(DIK_S, KEY_S));
	m_keyMap.insert(std::make_pair(DIK_D, KEY_D));
	m_keyMap.insert(std::make_pair(DIK_C, KEY_C));		// Reset Camera
	m_keyMap.insert(std::make_pair(DIK_F1, KEY_F1));	// Screenshot
}

//------------------------------------------------------------------------
// Purpose  : Initialize Input Devices
//------------------------------------------------------------------------
bool InputDevice::Initialize(HWND hwnd, HINSTANCE hInstance)
{
	
	// Get Direct Input Pointer
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, 0)))
	{
		return false;
	}

	//  Create Keyboard
	ZeroMemory(m_KeyboardState, sizeof(m_KeyboardState));
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0)))
	{
		return false;
	}
	else
	{
		// Set data format
		m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pKeyboard->Acquire();
		LogManager::GetInstance()->WriteToFile("Keyboard acquired...");

		MapKeycodes();
	}
	
	
	// Create Mouse	
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, 0)))
	{
		return false;
	}
	else
	{
		// Set data format
		m_pMouse->SetDataFormat(&c_dfDIMouse2);
		m_pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		m_pMouse->Acquire();
		LogManager::GetInstance()->WriteToFile("Mouse acquired...");
	}

	return true;
}

//------------------------------------------------------------------------
// Date     : 19/7/2009
// Purpose  : Release all COM objects!
//------------------------------------------------------------------------
void InputDevice::ShutDown()
{
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
		m_pKeyboard = 0;
		LogManager::GetInstance()->WriteToFile("Keyboard unacquired...");
	}

	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = 0;
		LogManager::GetInstance()->WriteToFile("Mouse unacquired...");
	}

	SAFE_RELEASE(m_pInput);
	LogManager::GetInstance()->WriteToFile("InputDevice Shutdown...");
}

//------------------------------------------------------------------------
// Purpose  : Let the POLLing begin...
//------------------------------------------------------------------------
void InputDevice::Poll()
{
	HRESULT Hr;

	// Poll Keyboard
	if (m_pKeyboard)
	{
		Hr = m_pKeyboard->GetDeviceState(sizeof(m_KeyboardState), (void**)&m_KeyboardState);
		
		if (FAILED(Hr))
		{
			// Keyboard lost ...
			ZeroMemory(m_KeyboardState, sizeof(m_KeyboardState));

			// try to acquire next time we poll
			Hr = m_pKeyboard->Acquire();
		}
		else
		{
			std::map<char, InputState>::iterator iter = m_keyMap.begin();
			
			for ( ; iter != m_keyMap.end() ; iter++)
			{
				if(KeyDown(iter->first))
					notifyObserver(iter->second);
			}
		}
	}
	
	// Poll Mouse...
	if (m_pMouse)
	{
		Hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_MouseState);

		if (FAILED(Hr))
		{
			// Mouse Lost...
			ZeroMemory(&m_MouseState, sizeof(m_MouseState));

			// try to acquire next time we poll
			Hr = m_pMouse->Acquire();
		}
		else
		{
			// We look for first three entries in the InputState enum
			// which holds Mouse Buttons
			for (int i = 0 ; i < 3 ; i++)
			{
				if (MouseButtonDown(i))
					notifyObserver(InputState(i));
			}
		}
	}
}

//------------------------------------------------------------------------
// Purpose  : Handle Keyboard input
//------------------------------------------------------------------------
bool InputDevice::KeyDown(char key)
{
	// SMALL EXPLANATION : A key is defined to be down if the bits 0x80 are 
	// set in the element of mKeyboardState that corresponds to the particular 
	// key we are checking. For example, the Escape key is down if the 
	// following is true:
	//
	//	(mKeyboardState[DIK_ESCAPE] & 0x80) != 0;


	return (m_KeyboardState[key] & 0x80) != 0;	
}

//------------------------------------------------------------------------
// Purpose  : Handle Mouse Inputs
//------------------------------------------------------------------------
bool InputDevice::MouseButtonDown(int button)
{
	return ( m_MouseState.rgbButtons[button] & 0x80 ) != 0;
}

float InputDevice::MouseDx()
{
	return (float)m_MouseState.lX;
}

float InputDevice::MouseDy()
{
	return (float)m_MouseState.lY;
}

float InputDevice::MouseDz()
{
	return (float)m_MouseState.lZ;
}

//------------------------------------------------------------------------
// Purpose  : Register the Observers
//------------------------------------------------------------------------
void InputDevice::registerObserver( IObserver* observer )
{
	m_vecObserver.push_back(observer);
}

//------------------------------------------------------------------------
// Purpose  : Remove the Observer
//------------------------------------------------------------------------
void InputDevice::removeObserver( IObserver* observer )
{
	std::vector<IObserver*>::iterator iter = m_vecObserver.begin();

	for ( ; iter != m_vecObserver.end() ; iter++)
	{
		if( (*iter) == observer )
			m_vecObserver.erase(iter);
	}
}

//------------------------------------------------------------------------
// Purpose  : Send notifications to all registered observers!
//------------------------------------------------------------------------
void InputDevice::notifyObserver(InputState state)
{
	std::vector<IObserver*>::iterator iter = m_vecObserver.begin();

	for ( ; iter != m_vecObserver.end() ; iter++)
	{
		(*iter)->Update(state);
	}
}


