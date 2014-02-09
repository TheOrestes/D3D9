
#include <stdlib.h>
#include <d3dx9.h>
#include "LevelParser.h"
#include "../Misc/LogManager.h"
#include "../GeometrySystem/GameObject.h"
#include "../GeometrySystem/MeshObject.h"
#include "../LightingSystem/PointLightObject.h"
#include "../LightingSystem/DirectionalLightObject.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
LevelParser::LevelParser()
{

}

//------------------------------------------------------------------------
// Purpose  : Destructor
//------------------------------------------------------------------------
LevelParser::~LevelParser()
{
	Kill();
}

//------------------------------------------------------------------------
// Purpose  : Kill Me!
//------------------------------------------------------------------------
void	LevelParser::Kill()
{

}

//------------------------------------------------------------------------
// Purpose  : Remove starting & Ending brackets from the string
//------------------------------------------------------------------------
std::string LevelParser::RemoveBrackets(std::string& bracketString)
{
	int openBraceIndex = bracketString.find_first_of('[');

	// start substring from index next to '[' & since we are removing 2
	// braces from the original string, we minus 2 from string length!
	return bracketString.substr(openBraceIndex+1, bracketString.size()-2);	
}

//------------------------------------------------------------------------
// Purpose  : Parse the Level File
//------------------------------------------------------------------------
bool	LevelParser::ParseLVL(const std::string& lvlFile, std::vector<GameObject*>& vecGameObj)
{
	std::ifstream ifs(lvlFile.c_str());
	
	std::string currStr;

	// Environment Map stuff!
	std::string EnvMapTexturePath;

	if (ifs)
	{
		while (ifs)
		{
			ifs >> currStr;

			if (currStr == "SceneBegin")
			{
				std::getline(ifs, currStr);		// ignore rest of the line
			}
			//-- PARSE ENVIRONMENT MAP
			else if (currStr == "EnvironmentBegin")
			{
				std::getline(ifs, currStr);		// ignore rest of the line

				while (currStr != "EnvironmentEnd")
				{
					ifs >> currStr;

					if (currStr == "EnvironmentPath")
					{
						ifs >> currStr;
						EnvMapTexturePath = RemoveBrackets(currStr);
					}
				}
			}
			//-- PARSE MESHOBJECT
			else if (currStr == "MeshBegin")
			{
				MeshObject* tmpObj = new MeshObject();

				std::getline(ifs, currStr);		// ignore rest of the line

				while (currStr != "MeshEnd")
				{
					ifs >> currStr;

					if (currStr == "ObjectId")
					{
						ifs >> currStr;
						std::string objId = RemoveBrackets(currStr);
						
						// set model id
						static_cast<GameObject*>(tmpObj)->setObjectID(atof(objId.c_str()));
					}
					else if (currStr == "ObjectName")
					{
						ifs >> currStr;
						std::string objName = RemoveBrackets(currStr);

						// set model name
						static_cast<GameObject*>(tmpObj)->setObjectName(objName);
					}
					else if (currStr == "ObjectType")
					{
						ifs >> currStr;

						// set Object type
						static_cast<GameObject*>(tmpObj)->setObjectType(STATIC_MESH);
					}
					else if (currStr == "ObjectPath")
					{
						ifs >> currStr;
						std::string objPath = RemoveBrackets(currStr);

						// set model path
						tmpObj->setModelPath(objPath);
					}
					else if (currStr == "ObjectShader")
					{
						ifs >> currStr;
						std::string objShader = RemoveBrackets(currStr);

						// set object shader
						tmpObj->setModelShader(objShader);
					}
					else if (currStr == "ObjectPosition")
					{
						// This has data separated by "Spaces"	
						ifs >> currStr;
						std::string objPosX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objPosY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objPosZ = RemoveBrackets(currStr);

						// set model position
						D3DXVECTOR3 tmpPos;
						tmpPos.x = atof(objPosX.c_str());
						tmpPos.y = atof(objPosY.c_str());
						tmpPos.z = atof(objPosZ.c_str());

						static_cast<GameObject*>(tmpObj)->setObjectPos(tmpPos);
					}
					else if (currStr == "ObjectRotation")
					{
						// This has data separated by "Spaces"	
						ifs >> currStr;
						std::string objRotX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objRotY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objRotZ = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string angle = RemoveBrackets(currStr);

						// set model rotation
						D3DXVECTOR3 tmpRot;
						tmpRot.x = atof(objRotX.c_str());
						tmpRot.y = atof(objRotY.c_str());
						tmpRot.z = atof(objRotZ.c_str());
						float theta = atof(angle.c_str());

						// degrees to radians
						theta *= (D3DX_PI/180.0f);

						static_cast<GameObject*>(tmpObj)->setObjectRot(tmpRot, theta);
					}
					else if (currStr == "ObjectScale")
					{
						ifs >> currStr;
						std::string objScaleX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objScaleY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objScaleZ = RemoveBrackets(currStr);

						// set model scale
						D3DXVECTOR3 tmpSc;
						tmpSc.x = atof(objScaleX.c_str());
						tmpSc.y = atof(objScaleY.c_str());
						tmpSc.z = atof(objScaleZ.c_str());

						static_cast<GameObject*>(tmpObj)->setObjectScale(tmpSc);
					}
					else if (currStr == "ObjectVisibility")
					{
						ifs >> currStr;
						std::string objVisible = RemoveBrackets(currStr);

						// set model visibility
						if(atof(objVisible.c_str()) == 1)
						{
							static_cast<GameObject*>(tmpObj)->SetObjectVisiblilty(true);
						}
						else
						{
							static_cast<GameObject*>(tmpObj)->SetObjectVisiblilty(false);
						}
					}
					else if (currStr == "EmissiveColor")
					{
						ifs >> currStr;
						std::string objColX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColZ = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColW = RemoveBrackets(currStr);

						// set model scale
						D3DXVECTOR4 tmpCol;
						tmpCol.x = atof(objColX.c_str());
						tmpCol.y = atof(objColY.c_str());
						tmpCol.z = atof(objColZ.c_str());
						tmpCol.w = atof(objColW.c_str());

						tmpObj->setModelEmissiveCol(tmpCol);
					}
					else if (currStr == "AmbientColor")
					{
						ifs >> currStr;
						std::string objColX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColZ = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColW = RemoveBrackets(currStr);

						// set model scale
						D3DXVECTOR4 tmpCol;
						tmpCol.x = atof(objColX.c_str());
						tmpCol.y = atof(objColY.c_str());
						tmpCol.z = atof(objColZ.c_str());
						tmpCol.w = atof(objColW.c_str());

						tmpObj->setModelAmbientCol(tmpCol);
					}
					else if (currStr == "DiffuseColor")
					{
						ifs >> currStr;
						std::string objColX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColZ = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColW = RemoveBrackets(currStr);

						// set model scale
						D3DXVECTOR4 tmpCol;
						tmpCol.x = atof(objColX.c_str());
						tmpCol.y = atof(objColY.c_str());
						tmpCol.z = atof(objColZ.c_str());
						tmpCol.w = atof(objColW.c_str());

						tmpObj->setModelDiffuseCol(tmpCol);
					}
					else if (currStr == "SpecularColor")
					{
						ifs >> currStr;
						std::string objColX = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColY = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColZ = RemoveBrackets(currStr);
						ifs >> currStr;
						std::string objColW = RemoveBrackets(currStr);

						// set model scale
						D3DXVECTOR4 tmpCol;
						tmpCol.x = atof(objColX.c_str());
						tmpCol.y = atof(objColY.c_str());
						tmpCol.z = atof(objColZ.c_str());
						tmpCol.w = atof(objColW.c_str());

						tmpObj->setModelSpecularCol(tmpCol);
					}
				}

				// Update Environment Map related entries in our Model data structure!
				static_cast<GameObject*>(tmpObj)->setEnvMap(EnvMapTexturePath);

				// Add this Game Object in our list of all GameObjects!
				LogManager::GetInstance()->WriteToFile(static_cast<GameObject*>(tmpObj)->getObjectName().c_str(), " Pushed...");
				vecGameObj.push_back(static_cast<GameObject*>(tmpObj));
			}
			//-- PARSE LIGHT PROPERTIES
			else if (currStr == "LightBegin")
			{

				std::getline(ifs, currStr);		// ignore rest of the line

				ifs >> currStr;

				if (currStr == "ObjectType")
				{
					ifs >> currStr;
					std::string lightType = RemoveBrackets(currStr);

					if (!strcmp(lightType.c_str(), "PointLight"))
					{
						// Create a tmp point light object...
						PointLightObject* tmpLightObj = new PointLightObject();

						static_cast<GameObject*>(tmpLightObj)->setObjectType(POINT_LIGHT);

						while (currStr != "LightEnd")
						{
							ifs >> currStr;
							if (currStr == "ObjectId")
							{
								ifs >> currStr;
								std::string objId = RemoveBrackets(currStr);

								// set model id
								static_cast<GameObject*>(tmpLightObj)->setObjectID(atof(objId.c_str()));
							}
							else if (currStr == "ObjectName")
							{
								ifs >> currStr;
								std::string objName = RemoveBrackets(currStr);

								// set model name
								static_cast<GameObject*>(tmpLightObj)->setObjectName(objName);
							}
							if (currStr == "LightColor")
							{
								// This has data separated by "Spaces"	
								ifs >> currStr;
								std::string lightCol_r = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_g = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_b = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_a = RemoveBrackets(currStr);

								// set light color
								D3DXVECTOR4 tmpCol;
								tmpCol.x = atof(lightCol_r.c_str());
								tmpCol.y = atof(lightCol_g.c_str());
								tmpCol.z = atof(lightCol_b.c_str());
								tmpCol.w = atof(lightCol_a.c_str());

								tmpLightObj->setLightColor(tmpCol);
							}
							else if (currStr == "LightPosition")
							{
								// This has data separated by "Spaces"	
								ifs >> currStr;
								std::string lightPosX = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightPosY = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightPosZ = RemoveBrackets(currStr);

								// set model position
								D3DXVECTOR3 tmpPos;
								tmpPos.x = atof(lightPosX.c_str());
								tmpPos.y = atof(lightPosY.c_str());
								tmpPos.z = atof(lightPosZ.c_str());

								// set object shader
								tmpLightObj->setLightPosition(tmpPos);
							}
							else if (currStr == "LightIntensity")
							{
								ifs >> currStr;
								std::string lightIntensity = RemoveBrackets(currStr);

								tmpLightObj->setLightIntensity(atof(lightIntensity.c_str()));
							}
							else if (currStr == "LightRadius")
							{
								ifs >> currStr;
								std::string lightRange = RemoveBrackets(currStr);

								tmpLightObj->setLightRadius(atof(lightRange.c_str()));
							}
							else if (currStr == "LightAttenuation")
							{
								ifs >> currStr;
								std::string lightAtt0 = RemoveBrackets(currStr);

								tmpLightObj->setLightAtten(atoi(lightAtt0.c_str()));
							}
						}
						
						// Push LightObject as GameObject into the list!
						vecGameObj.push_back(static_cast<GameObject*>(tmpLightObj));
					}
					else if (!strcmp(lightType.c_str(), "DirectionalLight"))
					{
						// Create a tmp point light object...
						DirectionalLightObject* tmpLightObj = new DirectionalLightObject();

						static_cast<GameObject*>(tmpLightObj)->setObjectType(DIRECTIONAL_LIGHT);

						while (currStr != "LightEnd")
						{
							ifs >> currStr;
							if (currStr == "ObjectId")
							{
								ifs >> currStr;
								std::string objId = RemoveBrackets(currStr);

								// set model id
								static_cast<GameObject*>(tmpLightObj)->setObjectID(atof(objId.c_str()));
							}
							else if (currStr == "ObjectName")
							{
								ifs >> currStr;
								std::string objName = RemoveBrackets(currStr);

								// set model name
								static_cast<GameObject*>(tmpLightObj)->setObjectName(objName);
							}
							if (currStr == "LightColor")
							{
								// This has data separated by "Spaces"	
								ifs >> currStr;
								std::string lightCol_r = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_g = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_b = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightCol_a = RemoveBrackets(currStr);

								// set light color
								D3DXVECTOR4 tmpCol;
								tmpCol.x = atof(lightCol_r.c_str());
								tmpCol.y = atof(lightCol_g.c_str());
								tmpCol.z = atof(lightCol_b.c_str());
								tmpCol.w = atof(lightCol_a.c_str());

								tmpLightObj->setLightColor(tmpCol);
							}
							else if (currStr == "LightDirection")
							{
								// This has data separated by "Spaces"	
								ifs >> currStr;
								std::string lightDirX = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightDirY = RemoveBrackets(currStr);
								ifs >> currStr;
								std::string lightDirZ = RemoveBrackets(currStr);

								// set model position
								D3DXVECTOR3 tmpDir;
								tmpDir.x = atof(lightDirX.c_str());
								tmpDir.y = atof(lightDirY.c_str());
								tmpDir.z = atof(lightDirZ.c_str());

								// set object shader
								tmpLightObj->setLightDirection(tmpDir);
							}
							else if (currStr == "LightIntensity")
							{
								ifs >> currStr;
								std::string lightIntensity = RemoveBrackets(currStr);

								tmpLightObj->setLightIntensity(atof(lightIntensity.c_str()));
							}
						}

						// Push LightObject as GameObject into the list!
						vecGameObj.push_back(static_cast<GameObject*>(tmpLightObj));
					}
				}
			}
			if (currStr == "SceneEnd")
			{
				std::getline(ifs, currStr);		// ignore rest of the line
			}
			
		}
	}

	return true;
}

