// ***************************************************************
//  LevelParser			
//  -------------------------------------------------------------
//  Purpose :	Parses the "LVL" file containing scene information
// ***************************************************************

#ifndef LEVEL_PARSER_H
#define LEVEL_PARSER_H

#include <fstream>
#include <string>
#include <vector>

class GameObject;

class LevelParser
{
public:
	LevelParser();
	virtual	~LevelParser();

	bool			ParseLVL(const std::string& path, std::vector<GameObject*>& vecGameObj);
	std::string		RemoveBrackets(std::string& path);
	void			Kill();

private:
	LevelParser(const LevelParser&);
};

#endif
