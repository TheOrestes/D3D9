// ***************************************************************
//  TextParser			
//  -------------------------------------------------------------
//  Purpose :	Parse the text file for FileName, FilePath, Shader
//				Textures & all other possible data
// ***************************************************************


#include <fstream>
#include <string>
#include <map>
#include "../Misc/Helper.h"


class TextParser
{
public:
	TextParser();
	virtual	~TextParser();

	bool	ReadFile(const std::string& fileName);
	void	Kill();

	void	DoSomething();

	inline  std::map<std::string, Helper::StaticGeomData>	GetParserData(){ return m_mapParserData; }

private:
	TextParser(const TextParser&);
	std::map<std::string, Helper::StaticGeomData>	m_mapParserData;
};

