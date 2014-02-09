// ***************************************************************
//  HELPER			
//  -------------------------------------------------------------
//  Purpose :	All helper function at one location!
// ***************************************************************

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p)=NULL; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          { if (p) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef MIN
#define MIN(x,y)				( (x)<(y)? (x) : (y) )
#endif

#ifndef MAX
#define MAX(x,y)				( (x)>(y)? (x) : (y) )
#endif

namespace Helper
{
	
	// MATH Helpers
	inline float Lerp(float a, float b, float t)
	{
		return a - (a*t) + (b*t);
	}

	// COLOR Helpers
	inline D3DXCOLOR SAND_COLOR()
	{
		return D3DXCOLOR(223.0f, 191.0f, 96.0f, 0.0f);
	}
	
	inline D3DXCOLOR LIGHT_YELLOW_GREEN_COLOR()
	{
		return D3DXCOLOR(193.0f, 251.0f, 60.0f, 0.0f);
	}

	inline D3DXCOLOR GREEN_COLOR()
	{
		return D3DXCOLOR(51.0f, 145.0f, 40.0f, 0.0f);
	}

	inline D3DXCOLOR DARK_YELLOW_GREEN_COLOR()
	{
		return D3DXCOLOR(104.0f, 106.0, 4.0f, 0.0f);
	}

	inline D3DXCOLOR BROWN_COLOR()
	{
		return D3DXCOLOR(157.0f, 107.0f, 57.0f, 0.0f);
	}

	inline D3DXCOLOR SNOW_WHITE_COLOR()
	{
		return D3DXCOLOR(235.0f, 243.0f, 254.0f, 0.0f);
	}
	
	inline DWORD ftoDW( float f)
	{
		return *((DWORD*)&f);
	}

	inline float GetRandomFloat( float minRange, float maxRange )
	{
		if ( minRange >= maxRange )			// bad input!
			return minRange;

		float RandNum = float(rand())/RAND_MAX;

		return ( minRange + (maxRange - minRange)*RandNum );
	}

	inline void GetRandomVector( D3DXVECTOR3& out, const D3DXVECTOR3& min, const D3DXVECTOR3& max)
	{
		out.x = GetRandomFloat(min.x, max.x);
		out.y = GetRandomFloat(min.y, max.y);
		out.z = GetRandomFloat(min.z, max.z);
	}

	inline void GetRandomVec(D3DXVECTOR3& out)
	{
		out.x = GetRandomFloat(-1.0f, 1.0f);
		out.y = GetRandomFloat(-1.0f, 1.0f);
		out.z = GetRandomFloat(-1.0f, 1.0f);

		// Project onto unit sphere.
		D3DXVec3Normalize(&out, &out);
	}

	inline std::string ExtractDirectory(const std::string& path)
	{
		return path.substr(0, path.find_last_of('\\') + 1);
	}

	inline std::string ExtractRawFileName(const std::string& fileName)
	{
		int lastIndex = fileName.find_last_of(".");
		std::string rawName = fileName.substr(0, lastIndex);

		return rawName;
	}

	inline bool StringContains(const std::string& text, const std::string& text2Search)
	{
		size_t found;
		text.find(text2Search);
		if(found != std::string::npos)
			return true;
		else
			return false;
	}

	struct StaticGeomData 
	{
		StaticGeomData(){	path.clear(); shader.clear();	}
		StaticGeomData(std::string _path, std::string _shader){	path = _path; shader = _shader;	}
		
		std::string path;
		std::string shader;
	};
}

#endif