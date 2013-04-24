/*
 * Polygon.cpp
 *
 *  Created on: Nov 22, 2012
 *      Author: brzeszczot
 */

#include "Polygon.hpp"

Polygon::Polygon(int cwsp, int *wsp)
{
    n = cwsp;
    tmp.x = tmp.y = -1;
    //====================================
  //  p.x = 3;
  //  p.y = 4;
//  p.x = 76;
//  p.y = 100;
//int t[n*2] = {100,100,100,200,200,200};
//int t[10] = {4,0,0,4,3,7,6,5,2,4};
int max_x=0;
int j,i;
   for(j=i=0;i<n;i++)
   {
    polygon[i].x = wsp[j];
    if (polygon[i].x>max_x)
        max_x=polygon[i].x;
    polygon[i].y = wsp[j+1];

   // cout << polygon[i].x << "x" << polygon[i].y << ", ";

    j+=2;
    }

    n = n;
    r.x=max_x+1;
}

int Polygon::min(int a, int b)
{
   if (a<b) return a;
   return b;
}
int Polygon::max(int a, int b)
{
   if (a>b) return a;
   return b;
}
int Polygon::sign(int a)
{
   if (a == 0)
      return 0;
   if (a < 0)
      return -1;
   return 1;
}
int Polygon::przynaleznosc(vertex x, vertex y, vertex z)
{
    int	det;
    det=x.x*y.y+y.x*z.y+z.x*x.y-z.x*y.y-x.x*z.y-y.x*x.y;

    if (det!=0)
        return(0);
    else
    {
        if ((min(x.x,y.x)<=z.x)&&(z.x<=max(x.x,y.x)) && (min(x.y,y.y)<=z.y)&&(z.y<=max(x.y,y.y)))
            return(1);
        else
            return(0);
    }
}
int Polygon::det(vertex x, vertex y, vertex z)
{
    int ret = (x.x*y.y+y.x*z.y+z.x*x.y-z.x*y.y-x.x*z.y-y.x*x.y);
    //cout <<"ret: "<<ret<<", z.x: "<<z.x<<", z.y: "<<z.y<<", tmp.x: "<<tmp.x<<"tmp.y: "<<tmp.y<<endl;
    if((abs(z.x)>=300000 || abs(z.y)>=300000) || (z.x==-1 || z.y==-1))
        return 1;
    else
        return ret;
}
int Polygon::przecinanie(vertex a, vertex b)
{
    if ((przynaleznosc(p,r,a)==0)&&(przynaleznosc(p,r,b)==0))
    { //pólprosta nie przecina odcinka |AB| w koncach
    if ((sign(det(p,r,a)) != sign(det(p,r,b))) &&
        (sign(det(a,b,p)) != sign(det(a,b,r))))
        {
            return(1);
        }
        else return(0);
    } else //do pólprostej nalezy przynajmniej jeden koniec odcinka |AB|
        {
       if ((przynaleznosc(p,r,a)==1)&&(przynaleznosc(p,r,b)==1))
        {
          if ((sign(det(p,r,polygon[(k-1+n)%n])) == sign(det(p,r,polygon[(k+2)%n]))) &&
              (sign(det(p,r,polygon[(k-1+n)%n])) != 0)) return(0); else return(1);
          } else
          if ((przynaleznosc(p,r,polygon[(k-1+n)%n])==1)||(przynaleznosc(p,r,polygon[(k+2)%n])==1)) return(0);
            else
            { //polprosta zawiera tylko wierzcholek
                if (przynaleznosc(p,r,b)==1)
                {
                    tmp=a;
                    return(0);
                }
                if (przynaleznosc(p,r,a)==1)
                {
                    if ((sign(det(p,r,tmp)) == sign(det(p,r,b))) && (sign(det(p,r,tmp)) != 0))
                        return(0);
                    else
                        return(1);
                }
             }
       }
       return 0;
}
bool Polygon::oblicz(vertex point)
{
    int l = 0; //liczba przeciec
    int i;
    div_t w;

    p.x = point.x;
    p.y = point.y;
    r.y=p.y;

    for (i=0;i<n;i++)
    {
       k=i;
       if (przynaleznosc(polygon[i], polygon[(i+1)%n], p) == 1)
       {
       //   printf("Punkt nalezy do krawedzi wielokata\n");
       //   printf("Dowlony klawisz...");
          return true;
       }
       if (przecinanie(polygon[i],polygon[(i+1)%n])==1)
        l++;
    }
    //printf("Rozwiazanie--------------\n");
    //printf("Liczba przeciec: %i\n",l);
    w=div(l,2);
    if (w.rem==0)
        return false;//printf("Punkt p NIE nalezy do wielokata\n");
    else
        return true;
}



