// ***************************************************************
//  D3DHelper			
//  -------------------------------------------------------------
//  Purpose :	All DirectX related help functions
// ***************************************************************

namespace D3DHelper
{

	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
	const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
	const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);

	struct Mtrl
	{
		Mtrl():ambient(WHITE), diffuse(WHITE), spec(WHITE), specPower(8.0f){}
		Mtrl(const D3DXCOLOR& a, const D3DXCOLOR& d, const D3DXCOLOR& s, float power)
			:ambient(a), diffuse(d), spec(s), specPower(power){}

		D3DXCOLOR	ambient;
		D3DXCOLOR	diffuse;
		D3DXCOLOR	spec;
		float		specPower;
	};
}

