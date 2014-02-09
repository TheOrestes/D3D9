
#include "TextParser.h"
#include "../Misc/LogManager.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
TextParser::TextParser()
{
	m_mapParserData.clear();
}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
TextParser::~TextParser()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Read the TEXT data file
//------------------------------------------------------------------------
bool	TextParser::ReadFile(const std::string& fileName)
{
	std::map<std::string, Helper::StaticGeomData>   map_geomData;
	map_geomData.clear();


	std::string tempName;
	std::string tempPath;
	std::string tempShader;

	Helper::StaticGeomData tempData;

	std::ifstream inFile(fileName.c_str());

	if (inFile.fail())
	{
		// File not found
		LogManager::GetInstance()->WriteToFile(fileName.c_str(), "not found!!!");
		return false;
	}
	else
		LogManager::GetInstance()->WriteToFile(fileName.c_str(), "found & processing...");

	while(!inFile.eof())
	{
		getline(inFile, tempName, '\t');
		getline(inFile, tempPath, '\t');
		getline(inFile, tempShader);

		tempData.path = tempPath;
		tempData.shader	= tempShader;

		map_geomData.insert(make_pair(tempName, tempData));

		inFile.ignore(80, '\n');
	}  

	inFile.close();

	m_mapParserData = map_geomData;

	return true;
}

//------------------------------------------------------------------------
// Purpose  : 
//------------------------------------------------------------------------
void	TextParser::DoSomething()
{
	
}

//------------------------------------------------------------------------
// Purpose  : Everything must come to an END!
//------------------------------------------------------------------------
void	TextParser::Kill()
{
	m_mapParserData.clear();
	LogManager::GetInstance()->WriteToFile("TextParser Shutdown...");
}