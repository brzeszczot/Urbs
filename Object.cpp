/*
 * Object.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#include "Object.hpp"

Object::Object()
{
	enabled = true;
	obj_unused = false;

	params.life = MAX_LIFE;
	params.life_unit = WORK_UNIT;
	params.gold = 0;
	params.skills = 0;
	params.visible_dist = 20;
	params.range = 30;

	group = DEFAULT;
	owner = NULL;
	owner_group = 0;
}

Object::~Object()
{

}

void Object::unusedObject()
{
	// poki co dodatkowa grafika dla zuzytych zrodel jest dla jezior
	if(1)//group == Object::SOURCE_LAKE)
	{
		obj_unused = true;
		res_file += SOURCE_TYPES;
		isWall = false;
	}
	else
		enableObject(false);
}

void Object::enableObject(bool yes)
{
	if(yes)
	{
		enabled = true;
		isWall = true;
		visible = true;
	}
	else
	{
		enabled = false;
		isWall = false;
		visible = false;
	}
}
