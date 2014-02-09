#include "Screenshot.h"
#include "../RenderDevice/RenderDevice.h"
#include "../Misc/Helper.h"
#include "LogManager.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Screenshot::Screenshot()
{
	m_pSurface = NULL;
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
Screenshot::~Screenshot()
{
	SAFE_RELEASE(m_pSurface);
}

//------------------------------------------------------------------------
// Purpose  : Format the filename in increasing order
//------------------------------------------------------------------------
char* Screenshot::FormatFileName(const std::string& fileName, int count)
{
	char* buffer = new char[2];
	itoa(count, buffer, 10);

	char* name = (char*)malloc(sizeof(strlen(fileName.c_str())+strlen(buffer)));

	name = strcpy(name, fileName.c_str());
	name = strcat(name, buffer);

	return name;
}

//------------------------------------------------------------------------
// Purpose  : Take Screenshot
//------------------------------------------------------------------------
void Screenshot::TakeScreenshot()
{
	static int uScreenShotCounter = 1;
	char* zFileName = FormatFileName("Screenshot", uScreenShotCounter);
	char* ext = ".jpg";

	char* saveFileName  = (char*)malloc(sizeof(strlen(zFileName)+strlen(ext)));

	saveFileName = strcpy(saveFileName, zFileName);
	saveFileName = strcat(saveFileName, ext);

	IDirect3DDevice9* gDevice = RenderDevice::GetInstance()->getDevice();

	HRESULT Hr;

	// Get the display dimensions
	D3DDISPLAYMODE dispMode;
	Hr = gDevice->GetDisplayMode(0, &dispMode);

	// Create Image buffer to store the front buffer image
	gDevice->CreateOffscreenPlainSurface(dispMode.Width, dispMode.Height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pSurface, NULL);

	// read the front buffer into the image surface
	if (FAILED(gDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pSurface)))
	{
		m_pSurface->Release();
		LogManager::GetInstance()->WriteToFile("Read the front buffer into the image surface Failed!");
		return;
	}

	// write the entire surface to the requested file
	if (SUCCEEDED(D3DXSaveSurfaceToFile(saveFileName, D3DXIFF_JPG, m_pSurface, NULL, NULL)))
	{
		uScreenShotCounter++;
		LogManager::GetInstance()->WriteToFile(saveFileName, "file saved!");
	}

	// Release
	m_pSurface->Release();
}

void Screenshot::Update( InputState state )
{
	if(state == KEY_F1)
		TakeScreenshot();
}