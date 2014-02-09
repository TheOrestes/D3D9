// ***************************************************************
//  IObserver			
//  -------------------------------------------------------------
//  Purpose :	This interface is for Observers in the system.
// ***************************************************************

#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "InputState.h"

class IObserver
{
public:
	virtual void Update(InputState state) = 0;
};

#endif