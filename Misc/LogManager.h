// ***************************************************************
//  LogManager     
//  -------------------------------------------------------------
//  Purpose :	Generate a Log file for debugging purpose
// ***************************************************************

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class LogManager
{
public:
	
	virtual ~LogManager(){};

	// Singleton Instance
	static LogManager* GetInstance()
	{
		static LogManager inst;
		return &inst;
	}

	bool Initialize();
	void WriteToFile(const std::string& str);
	void WriteToFile(const std::string& str, const std::string& file, int line);
	void WriteToFile(const std::string& str1, const std::string& str2);
	void WriteToFile(const std::string& str, double);
	void WriteToFile(const std::string& str, float);
	void WriteToFile(const std::string& str, int);
	void Shutdown();

	ofstream	m_LogFile;

private:
	LogManager(){};
};

#endif
