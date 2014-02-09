// ***************************************************************
//  Screenshot 
//  -------------------------------------------------------------
//  Purpose : Take a screenshot
// ***************************************************************

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <d3dx9.h>
#include <string>
#include "../InputSystem/IObserver.h"

class Screenshot : public IObserver
{
public:
	Screenshot();
	virtual ~Screenshot();

	void		TakeScreenshot();
	char*		FormatFileName(const std::string& fileName, int count);

	void		Update(InputState state);

private:
	LPDIRECT3DSURFACE9 m_pSurface;
};

#endif