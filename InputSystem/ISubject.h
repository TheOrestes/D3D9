//------------------------------------------------------------------------
// Date    
// Purpose  : This is a general Interface for Subject in an Observer 
//			  pattern. Its an Abstract Class & every class that wants 
//			  to be Subject Class, should derive from this.
//------------------------------------------------------------------------

#ifndef ISUBJECT_H
#define ISUBJECT_H

#include "IObserver.h"
#include "InputState.h"

class ISubject
{
public:
	virtual void registerObserver(IObserver* observer) = 0;
	virtual void removeObserver(IObserver* observer) = 0;
	virtual void notifyObserver(InputState state) = 0;
};

#endif