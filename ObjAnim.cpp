/*
 * ObjAnim.cpp
 *
 *  Created on: Nov 18, 2012
 *      Author: brzeszczot
 */

#include "ObjAnim.hpp"

ObjAnim::ObjAnim()
{
	isWall = true;
	visible = true;
	border = false;

	animation.isAnimation = true;
	animation.animate = true;
	animation.reverse = false;
	animation.speed = 200;
	animation.timer = 0;
	animation.frame.x = 0;
	animation.frame.y = 0;
	animation.frame_xy.x = ANIM_FRAMES;
	animation.frame_xy.y = ANIM_TYPES;
	animation.rev_up = true;
	animation.multiline = false;

	resSetUp = true;
}

void ObjAnim::changeAnim(int current_time)
{
	time = current_time;
	if(time - animation.timer > animation.speed && animation.animate && animation.isAnimation)
	{
		if(animation.rev_up)
			animation.frame.x++;
		else
			animation.frame.x--;

		// jesli animacja dwustronna
		if(animation.reverse && (animation.frame.x >= animation.frame_xy.x-1 || animation.frame.x == 0))
			animation.rev_up = !animation.rev_up;
		// animacja jednostronna
		else if(animation.frame.x >= animation.frame_xy.x && !animation.reverse)
			animation.frame.x = 0;

		animation.timer = time;
	}
}

ObjAnim::~ObjAnim()
{
}
