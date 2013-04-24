/*
 * Vertex.hpp
 *
 *  Created on: Nov 28, 2012
 *      Author: brzeszczot
 */

#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#define DEFAULT_WORLD_OWNER 0
#define DEFAULT_HUMAN_OWNER 1

#define DIRECTION_UP		0
#define DIRECTION_RIGHT		1
#define DIRECTION_DOWN		2
#define DIRECTION_LEFT		3

#include <string>
#include <vector>
#include <ctime>
#include <maxtoa.h>

namespace Adds
{
	typedef struct vertex { int x, y; } vertex;
	unsigned int getDirection(Adds::vertex p1, Adds::vertex p2);	// oblicz pozycje p2 wzgledem p1 - ret 0-4 kier. swiata
	void getCornerCoord(Adds::vertex coords, Adds::vertex size, unsigned int corner, Adds::vertex &vert);
	bool checkOverlap(Adds::vertex vert, Adds::vertex coords, Adds::vertex size);
	void getDestCornerCord(int corner, Adds::vertex coords, Adds::vertex col_size, Adds::vertex obj_size, Adds::vertex &res);
	const std::string getStrDate(time_t now);
	const std::string getStrTime(time_t now);
};


#endif /* VERTEX_HPP_ */
