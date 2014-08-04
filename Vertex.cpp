/*
 * Vertex.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: brzeszczot
 */

#include "Vertex.hpp"

using namespace std;

Vertex::Vertex(int a,int b)
{
    x = a;
    y = b;
}

Vertex Vertex::operator=(Vertex a)
{
    x = a.x;
    y = a.y;
    return *this;
}

bool Vertex::operator==(Vertex a)
{
    return (x == a.x && y == a.y);
}

Vertex Vertex::operator+(Vertex a)
{
	a.x = x + a.x;
	a.y = y + a.y;
    return a;
}

Vertex Vertex::operator-(Vertex a)
{
	a.x = x - a.x;
	a.y = y - a.y;
    return a;
}

Vertex Vertex::operator*(Vertex a)
{
	a.x = x * a.x;
	a.y = y * a.y;
    return a;
}

Vertex Vertex::operator/(Vertex a)
{
	a.x = x / a.x;
	a.y = y / a.y;
    return a;
}

