// ***************************************************************
//  InputManager		
//  -------------------------------------------------------------
//  Purpose : In this class we are trying to implement an Observer
//			  pattern. IInputManager class is an interface, which 
//			  will be implemented by InputDevice Class. 
//
//			  Just like the definition of Observer pattern, This 
//			  class acts as a Subject having methods to register, 
//			  remove & notify the objects. 
//			
//			  So, every class that needs to process an Input info,
//			  should register as Objects & then IInputManager will
//			  take care of providing the necessary information about
//			  current input device state as & when its updated.
// ***************************************************************

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "ISubject.h"

class IInputManager : public ISubject
{
public:

	
};

#endif