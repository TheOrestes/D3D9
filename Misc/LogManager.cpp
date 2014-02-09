#include "LogManager.h"
#include <ctime>

//------------------------------------------------------------------------
// Purpose  : Initialize Log Manager
//------------------------------------------------------------------------
bool LogManager::Initialize()
{
	// Get Current System Date
	std::time_t tt = std::time(0);
	std::tm ttm = *std::localtime(&tt);

	m_LogFile.open("Log\\iWorld® Log.txt", ios::trunc);

	if (m_LogFile.is_open())
	{
		m_LogFile<<"====================================================================";
		m_LogFile<<"\n\tiWorld® Log";
		m_LogFile<<"\n\tDirectx Verison 9.0c (June 2010)";
		m_LogFile<<"\n\tAuthor : DoD";
		m_LogFile<<"\n\t"<<ttm.tm_mday<<"/"<<ttm.tm_mon+1<<"/"<<ttm.tm_year+1900<<"\t\t"<<ttm.tm_hour<<":"<<ttm.tm_min<<":"<<ttm.tm_sec;
		m_LogFile<<"\n====================================================================";
		return true;
	}
	else
		return false;
}

//------------------------------------------------------------------------
// Purpose  : Write Log to the file
//------------------------------------------------------------------------
void LogManager::WriteToFile(const std::string& str)
{
	m_LogFile<<"\n>>"<<str;
	m_LogFile.flush();
}

//////////////////////////////////////////////////////////////////////////////////////////
void LogManager::WriteToFile( const std::string& str, const std::string& file, int line )
{
	m_LogFile<<"\n>>"<<str;
	m_LogFile<<"\n>>--->File : "<<file<<"\tLine : "<<line;
	m_LogFile.flush();
}

//------------------------------------------------------------------------ 
// Purpose  : Concat & write to file
//------------------------------------------------------------------------
void LogManager::WriteToFile(const std::string& str1, const std::string& str2)
{
	m_LogFile<<"\n>>"<<str1;
	m_LogFile<<" "<<str2;
	m_LogFile.flush();
}

//------------------------------------------------------------------------  
// Purpose  : Concat & write double to file
//------------------------------------------------------------------------
void LogManager::WriteToFile(const std::string& str1, double val)
{
	char x[50];
	sprintf(x,"%13.5f",val);

	m_LogFile<<"\n>>"<<str1;
	m_LogFile<<" "<<x;
	m_LogFile.flush();
}

//------------------------------------------------------------------------  
// Purpose  : Concat & write float to file
//------------------------------------------------------------------------
void LogManager::WriteToFile(const std::string& str1, float val)
{
	char x[50];
	sprintf(x,"%13.5f",val);

	m_LogFile<<"\n>>"<<str1;
	m_LogFile<<" "<<x;
	m_LogFile.flush();
}

//------------------------------------------------------------------------
// Purpose  : Concat & write int to file
//------------------------------------------------------------------------
void LogManager::WriteToFile(const std::string& str1, int val)
{
	char x[50];
	sprintf(x,"%13.5d",val);

	m_LogFile<<"\n>>"<<str1;
	m_LogFile<<" "<<x;
	m_LogFile.flush();
}

//------------------------------------------------------------------------
// Purpose  : Every good thing must come to an end ..Shutdown LogManager!
//------------------------------------------------------------------------
void LogManager::Shutdown()
{
	if (m_LogFile.is_open())
	{
		m_LogFile<<"\n====================================================================";
		m_LogFile<<"\n\t\t\t\t\t\t\t   THE END";
		m_LogFile<<"\n====================================================================";
	}

	m_LogFile.close();
}