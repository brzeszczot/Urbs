/*
 * Polygon.hpp
 *
 *  Created on: Nov 22, 2012
 *      Author: brzeszczot
 */

#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <cstdlib>
using namespace std;

class Polygon
{
    public:
        typedef struct vertex 			//Wierzcholek
        {
            int x,y;
        };
        vertex 	polygon[2000];	//Tablica wierzcholow wielokata, max: 20
        vertex 	p;				//Dany punkt p
        vertex 	r;				//Drugi koniec odcinka |PR|
        vertex 	tmp;
        int n;			    //Liczba wierzchalkow wielokata
        int k;

        Polygon(int cwsp, int *wsp);
        int min(int a, int b);
        int max(int a, int b);
        int sign(int a);
        int przynaleznosc(vertex x, vertex y, vertex z);
        int det(vertex x, vertex y, vertex z); //Wyznacznik macierzy kwadratowej stopnia 3.
        int przecinanie(vertex a, vertex b);
        bool oblicz(vertex point);
};

#endif /* POLYGON_HPP_ */
