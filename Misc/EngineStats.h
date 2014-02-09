// ***************************************************************
//  EngineStats    
//  -------------------------------------------------------------
//  Purpose :  Keep record of Engine Statistics like, FPS
//			   triangles being rendered etc.
// ***************************************************************

#ifndef ENGINE_STATS_H
#define ENGINE_STATS_H

#include <d3dx9.h>

class EngineStats
{
public:
	
	virtual ~EngineStats();

	static EngineStats* GetInstance()
	{
		static EngineStats inst;
		return &inst;
	}

	void	onLostDevice();
	void	onResetDevice();
	
	bool	Initialize();
	void	Update(float dt);
	void	Render();

	void	addVertices(DWORD n);
	void	subVertices(DWORD n);
	void	addTriangles(DWORD n);
	void	subTriangles(DWORD n);
	void	setTriCount(DWORD n);
	void	setVertexCount(DWORD n);

private:
	EngineStats(const EngineStats&);					// Explicitly disallow the use of 
	EngineStats operator=(const EngineStats&);			// compiler-generated functions 

	EngineStats();

private:
	ID3DXFont*	m_Font;
	float		m_FPS;
	float		m_msPerFrame;
	float		m_delta;
	DWORD		m_numTriangles;
	DWORD		m_numVertices;
	int			m_iDays;
	int			m_iMonths;
	int			m_iYears;
	int			m_iHours;
	int			m_iMinutes;
	int			m_iSeconds;
};
#endif
