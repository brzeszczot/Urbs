/*
 * Vertex.hpp
 *
 *  Created on: Apr 24, 2013
 *      Author: brzeszczot
 */

#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <algorithm>
#include <math.h>

class Vertex
{
	public:
		int x, y;
		Vertex() { x=0; y=0; }
		Vertex(int a, int b);
		Vertex operator+(Vertex a);
		Vertex operator-(Vertex a);
		Vertex operator*(Vertex a);
		Vertex operator/(Vertex a);
		bool operator==(Vertex a);
		Vertex operator=(Vertex a);
};

#endif /* VERTEX_HPP_ */
