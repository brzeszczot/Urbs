/*
 * Alg.cpp
 *
 *  Created on: Nov 22, 2012
 *      Author: brzeszczot
 */

#include "Alg.hpp"

using namespace std;

bool Algorithms::multilane(Adds::vertex p1, Adds::vertex p2, Adds::vertex v)
{
	float det = ((p1.x*p2.y)+(p2.x*v.y)+(v.x*p1.y))-(v.x*p2.y)-(p1.x*v.y)-(p2.x*p1.y);
    if(det<=0)
    	return true;
    else if(det>0)
    	return false;
}

float Algorithms::SectionLength(Adds::vertex v1, Adds::vertex v2)
{
    return abs(sqrt(pow((float)max(v2.x,v1.x)-min(v2.x,v1.x),2)+pow((float)max(v2.y,v1.y)-min(v2.y,v1.y),2)));
}

bool Algorithms::ObjectVisible(Adds::vertex p1, Adds::vertex p2, Adds::vertex v1, Adds::vertex v2)
{
	Adds::vertex v, p_min;
    bool allow = false; // glowny przelacznik okreslajacy czy przeszkoda znajduje sie pomiedzy porownywanymi obiektamy czy nie (true - tak, jest pomiedzy)
    int colision, x_domin, y_domin;
    float vmin = 0;
    x_domin = y_domin = colision = 0;

    // analizujemy kolejne wierzcholki prostokata
    for(int jj=0;jj<4;jj++)
    {
        switch(jj)
        {
            case 0: v = v1; break;
            case 1: v.x += v2.x; break;
            case 2: v.y += v2.y; break;
            case 3: v.x -= v2.x; break;
        }
        // wspoliniowosc - punkt po lewej lub prawej stronie odcinka
        float det = ((p1.x*p2.y)+(p2.x*v.y)+(v.x*p1.y))-(v.x*p2.y)-(p1.x*v.y)-(p2.x*p1.y);

        if(det<0)   colision--;
        else if(det>=0)   colision++;
        // okreslenie czy rozpatrywac polozenie obiekt przeszkoda wzgledem wsp X czy Y
        if(v.x>p2.x)    x_domin++;
        else            x_domin--;
        if(v.y>p2.y)    y_domin++;
        else            y_domin--;
        // dlugosc odcinka
        det = Algorithms::SectionLength(p2,v);
        // szulkanie ktory z 4 wierzcholkow znajduje sie najblizej obiektu
        // ustaw pierwsza wartosc do daleszego porownywania
        if(vmin==0 || det<vmin)
        {
            p_min = v;
            vmin = det;
        }
        // czy punkt znajduje sie w obszarze
//      if((min(p1.x,p2.x)<=v.x)&&(v.x<=max(p1.x,p2.x)) || (min(p1.y,p2.y)<=v.y)&&(v.y<=max(p1.y,p2.y)))
//      belong++;
    }
    // sprawdzanie czy przeszkoda znajduje sie pomiedzy badanymi obiektami czy nie
    if((x_domin==4 && y_domin==4 && p1.x>p_min.x && p1.y>p_min.y) || (x_domin==-4 && y_domin==-4 && p1.x<p_min.x && p1.y<p_min.y))
        allow = true;
    else if((x_domin==-4 && y_domin==4 && p1.x<p_min.x && p1.y>p_min.y) || (x_domin==4 && y_domin==-4 && p1.x>p_min.x && p1.y<p_min.y))
        allow = true;
    else if((x_domin==4 && abs(y_domin)!=4 && p1.x>p_min.x) || (x_domin==-4 && abs(y_domin)!=4 && p1.x<p_min.x) || (y_domin==4 && abs(x_domin)!=4 && p1.y>p_min.y) || (y_domin==-4 && abs(x_domin)!=4 && p1.y<p_min.y))
        allow = true;
     //cout << colision << ", " << allow << ", " << x_domin << ", " << y_domin << endl;
    //printf("%d, %d, %d, %d\n", colision, allow, x_domin, y_domin);

    // przeszkoda zaslania widok miedzy obiektami
    if(abs(colision)!=4 && allow)
        return false;
    // przeszkoda nie przeszkadza we wzajemnym wiedzeniu sie obiektow - zapisz ostatnio widziane wspolrzedne glownej postaci
    else
        return true;
}

Adds::vertex Algorithms::SectionCalc(Adds::vertex p1, Adds::vertex p2, int bytesPerLine, vector<Adds::vertex> *v, int deep)
{
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;
	int x2 = (int)p2.x;
	int y2 = (int)p2.y;

	Adds::vertex vtemp;
    int d, dx, dy, ai, bi, xi, yi;
    int x = x1, y = y1;
    // determining the direction of drawing
    if (x1 < x2)
    {
        xi = bytesPerLine;
        dx = x2 - x1;
    }
    else
    {
        xi = -bytesPerLine;
        dx = x1 - x2;
    }
    // determining the direction of drawing
    if (y1 < y2)
    {
        yi = bytesPerLine;
        dy = y2 - y1;
    }
    else
    {
        yi = -bytesPerLine;
        dy = y1 - y2;
    }
    // first pixel

    vtemp.x = x;
    vtemp.y = y;
    v->push_back(vtemp);
    if((int)v->size()>=deep && deep!=-1) return vtemp;
    //SetPixel(x, y, color);
    // the leading axis OX
    if (dx > dy)
    {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;
        // loop for x
        while (x <= (x2-bytesPerLine)||x > (x2+bytesPerLine))
        {
            // factor test
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                x += xi;
            }

            vtemp.x = x;
            vtemp.y = y;
            v->push_back(vtemp);
            //SetPixel(x, y, color);
            if((int)v->size()>=deep && deep!=-1) return vtemp;
        }
    }
    // the leading axis OY
    else
    {
        ai = (dx - dy) * 2;
        bi = dx * 2;
        d = bi - dy;
        // loop for y
        while (y <= (y2-bytesPerLine)||y > (y2+bytesPerLine))
        {
            // factor test
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                y += yi;
            }

            vtemp.x = x;
            vtemp.y = y;
            v->push_back(vtemp);
            //SetPixel(x, y, color);
            if((int)v->size()>=deep && deep!=-1) return vtemp;
        }
    }
    return vtemp;
}





