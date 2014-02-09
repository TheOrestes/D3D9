// ***************************************************************
//  IObject		
//  -------------------------------------------------------------
//  Purpose : Interface Class for Objects
// ***************************************************************

#ifndef IOBJECT_H
#define IOBJECT_H


class IObject
{
public:
	virtual ~IObject(){}

	virtual		void Update(float dt) = 0;
	virtual		void Render() = 0;
};

#endif